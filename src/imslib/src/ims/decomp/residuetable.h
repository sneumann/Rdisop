#ifndef IMS_RESIDUETABLE_H
#define IMS_RESIDUETABLE_H

namespace ims {

template <typename ValueType, typename DecompositionValueType>
class ResidueTable {
	public:
		typedef ValueType value_type;
		typedef DecompositionValueType decomposition_value_type;

		/** Type of output for One Decomposition Problem. */
		typedef std::vector<decomposition_value_type> decomposition_type;

		typedef typename decomposition_type::size_type size_type;

		/** Type of output for All Decompositions Problem. */
		typedef std::vector<decomposition_type> decompositions_type;

		typedef std::vector< std::pair<size_type, decomposition_value_type> > witness_vector_type;
		
		/** Type of rows of residues table. */
		typedef std::vector<value_type> row_type;

		/** Type of the residues table. */
		typedef std::vector<row_type> table_type;

		witness_vector_type getOneWitness() const { return witness_vector; }

		/** Default constructor */
		ResidueTable() : infty(0) { }

		/**
		 * Constructor for the ResidueTable. Currently the object
		 * can be thrown away as soon as the constructor has finished
		 * since the actual table is not kept in this class.
		 */
		ResidueTable(const Weights& weights,
					row_type& lcms,
					row_type& mass_in_lcms,
					table_type& table);

		
		value_type infinity() const { return infty; }

	private:
		void fill(
			const Weights& weights,
			row_type& lcms,
			row_type& mass_in_lcms,
			table_type& table);

		value_type infty;

		/**
		 * List of the witnesses that is used to find one mass decomposition.
		 * Corresponds to the witness vector w in the paper.
		 */

		/** A single witness (constructed while the table was created). */
		witness_vector_type witness_vector;
};


template <typename ValueType, typename DecompositionValueType>
ResidueTable<ValueType,DecompositionValueType>::ResidueTable(
	const Weights& weights,
	row_type& lcms,
	row_type& mass_in_lcms,
	table_type& table)
{
	assert(weights.size() > 0);
	infty = weights[0] * weights.back();
	fill(weights, lcms, mass_in_lcms, table);
}

template <typename ValueType, typename DecompositionValueType>
void ResidueTable<ValueType,DecompositionValueType>::fill(
		const Weights& weights,
		row_type& lcms,
		row_type& mass_in_lcms,
		table_type& table
)
{
	if (weights.size() < 2) {
		return;
	}
	// cache the most often used mass - smallest mass
	value_type smallestMass = weights.getWeight(0);
	value_type secondMass = weights.getWeight(1);

	// initialize table: infinity everywhere except in the first field of every column
	table.reserve(weights.size());
	table.assign(weights.size(), std::vector<value_type>(smallestMass, infty));

	for (size_type i = 0; i < weights.size(); ++i) {
		table[i][0] = 0;
	}

	// initialize witness vector
	witness_vector.reserve(smallestMass);
	witness_vector.resize(smallestMass);

	// fill second column (the first one is already correct)
	size_type it_inc = secondMass % smallestMass, witness = 1;
	typename row_type::iterator it = table[1].begin() + it_inc;
	value_type mass = secondMass;
	// counter for creation one witness vector
	decomposition_value_type counter = 0;
	while (it != table[1].begin()) {
		*it = mass;
		mass += secondMass;
		++counter;
		witness_vector[it - table[1].begin()] = std::make_pair(witness, counter);
		it += it_inc;
		if (it >= table[1].end()) {
			it -= table[1].size();
		}
	}
	// fill cache variables for i==1
	value_type d = gcd(smallestMass, secondMass);
	lcms[1] = secondMass*smallestMass / d;
	mass_in_lcms[1] = smallestMass / d;

	// fill remaining table. i is the column index.
	for (size_type i = 2; i < weights.size(); ++i) {
		// cache often used i-th alphabet mass
		value_type currentMass = weights.getWeight(i);
//		std::cout << "Current alphabet mass = " << currentMass << std::endl;

		value_type d = gcd(smallestMass, currentMass);
//		std::cout << "gcd = " << d << std::endl;

		// Fill cache for various variables.
		// Note that values for i==0 are never assigned since they're unused anyway.
		lcms[i] = currentMass*smallestMass / d;
		mass_in_lcms[i] = smallestMass / d;

		// Nijenhuis' improvement: Is currentMass composable with smaller alphabet?
		if (currentMass >= table[i-1][currentMass % smallestMass]) {
//			std::cout << "Nijenhuis improvement: current alphabet mass is bigger than " << table.at(i-1).at(currentMass % smallestMass) << std::endl;
			table[i] = table[i-1];
			continue;
		}

		const row_type& prev_column = table[i-1];
		row_type& cur_column = table[i];

		if (d == 1) {
			// This loop is for the case that the gcd is 1. The optimization used below
			// is not applicable here.

			// p_inc (as i understood) is used to change residue (p) efficiently
			size_type p_inc = currentMass % smallestMass;
//			std::cout << "p_inc = " << p_inc << std::endl;

			// n is the value that will be written into the table
			value_type n = 0;
			// current residue (in paper variable 'r' is used)
			size_type p = 0;
			// counter for creation one witness vector
			decomposition_value_type counter = 0;

			for (size_type m = smallestMass; m > 0; --m) {
				n += currentMass;
				p += p_inc;
				++counter;
				if (p >= smallestMass) {
					p -= smallestMass;
				}
				if (n > prev_column[p]) {
					n = prev_column[p];
					counter = 0;
				} else {
					witness_vector[p] = std::make_pair(i, counter);
				}
				cur_column[p] = n;
			}
		}
		else {
			// If we're here, the gcd is not 1. We can use the following cache-optimized
			// version of the algorithm. The trick is to put the iteration over all
			// residue classes into the _inner_ loop.
			//
			// One could see it as going through one column in blocks which are gcd entries long.
			size_type cur = currentMass % smallestMass;
			size_type prev = 0;
			size_type p_inc = cur-d;
			// counters for creation one witness vector
			std::vector<decomposition_value_type> counters(smallestMass);

			// copy first block from prev_column to cur_column
			for (size_type j = 1; j < d; ++j) {
				cur_column[j] = prev_column[j];
			}

			// first loop: Go through all blocks, updating cur_column for the first time.
			for (size_type m = smallestMass / d; m > 1; m--) {
				// r: current residue class
				for (size_type r = 0; r < d; r++) {

					++counters[cur];
					if (cur_column[prev] + currentMass > prev_column[cur]) {
						cur_column[cur] = prev_column[cur];
						counters[cur] = 0;
					} else {
						cur_column[cur] = cur_column[prev] + currentMass;
						witness_vector[cur] = std::make_pair(i, counters[cur]);
					}

					prev++;
					cur++;
				}

				prev = cur - d;

				// this does: cur = (cur + currentMass) % smallestMass - d;
				cur += p_inc;
				if (cur >= smallestMass) {
					cur -= smallestMass;
				}
			}

			// second loop:
			bool cont = true;
			while (cont) {
				cont = false;
				prev++;
				cur++;
				++counters[cur];
				for (size_type r = 1; r < d; ++r) {
					if (cur_column[prev] + currentMass < cur_column[cur]) {
						cur_column[cur] = cur_column[prev] + currentMass;
						cont = true;
						witness_vector[cur] = std::make_pair(i, counters[cur]);
					} else {
						counters[cur] = 0;
					}
					prev++;
					cur++;
				}

				prev = cur - d;

				cur += p_inc;
				if (cur >= smallestMass) {
					cur -= smallestMass;
				}
			}
		}
	}
}

} // namespace ims

#endif
