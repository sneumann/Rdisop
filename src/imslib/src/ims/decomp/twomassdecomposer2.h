#ifndef IMS_TWOMASSDECOMPOSER2_H
#define IMS_TWOMASSDECOMPOSER2_H

#include <vector>
#include <limits>
#include <cassert>
#include <ims/weights.h>
#include <ims/alphabet.h>
#include <ims/utils/gcd.h>
#include <ims/base/exception/invalidargumentexception.h>
#include <ims/decomp/residuetable.h>


namespace ims {

/*
There is some conditionally compiled code in this file depending on whether
or not the preprocessor macro CHECK_A is defined.

If defined, a second residue table for weight set A will be created and during
the recursion will be used to check whether the current mass can be decomposed over
the weight set A. If not, the recursion isn't done.
*/

/*
 There are three weight sets that must be distinguished:
 Weight set A and weight set B are the input weight sets provided by the user.
 A third weight set C is internally derived from set A and B.

 The code in this file uses the convention that a suffix _a, _b or _c
 (mnemonic: _c_ombined) is used to distinguish which weight set a variable
 belongs to.

 Of course this scheme can't be used as soon as more than two weight sets are
 supported.


Notes

map_ca[i] = (i < j) ? i : (i+1);

*/

/**
The specific implementation of @c MassDecomposer interface using algorithms and data structure that are described in the paper "Efficient Mass Decomposition" S. Böcker, Zs. Lipták, ACM SAC-BIO, 2004. The main idea is instead of using the classical dynamic programming algorithm with corresponding data structures, to store the residues of the smallest decomposable numbers for every modulo of the smallest alphabet mass. For a more detailed description please refer to the mentioned paper.
 * @ingroup decomp
 */
template <typename ValueType = long unsigned int,
		  typename DecompositionValueType = unsigned int>
class TwoMassDecomposer2 {
	public:
		typedef ValueType value_type;
		typedef DecompositionValueType decomposition_value_type;

		/**
		 * Type of output for One Decomposition Problem.
		 */
		typedef std::vector<decomposition_value_type> decomposition_type;

		typedef typename decomposition_type::size_type size_type;

		/**
		 * Type of output for All Decompositions Problem.
		 */
		typedef std::vector<decomposition_type> decompositions_type;

		/**
		 * Constructor.
		 *
		 * @param weights Weights over which the mass is to be decomposed.
		 */
		TwoMassDecomposer2(const Weights& weights_a, const Weights& weights_b);

		decompositions_type getAllDecompositions(value_type mass_a, value_type mass_b);

	private:
		/** Type of rows of residues table. */
		typedef std::vector<value_type> row_type;

		/** Type of the residues table. */
		typedef std::vector<row_type> table_type;

		/**
		 * Weights over which the mass is to be decomposed. Note that except
		 * in the constructor, weights_a always refers to the @b permuted
		 * weight set.
		 */
		Weights weights_a;
		Weights weights_c; ///< combined weights

		/** j is chosen such that weights_a[j]/weights_b[j] is minimal */
		size_type j;

		value_type weights_b_j; ///< weights_b isn't needed, just weights_b[j]

		/** Residue table for weights_c. */
		ResidueTable<ValueType,DecompositionValueType> rt_c;
#ifdef CHECK_A
		ResidueTable<ValueType,DecompositionValueType> rt_a;
#endif

		/** (data for residue table, probably to be moved into ResidueTable itself) */
		table_type ertable_c;
#ifdef CHECK_A
		table_type ertable_a;
#endif

		/** Least common multiples. */
		row_type lcms_c;
#ifdef CHECK_A
		row_type lcms_a;
#endif

		/**
		 * List of the counters for the least common multiples that store the number
		 * how often the smallest alphabet mass fits into the correcponding least
		 * common multiple (lcm).
		 */
		row_type mass_in_lcms_c;
#ifdef CHECK_A
		row_type mass_in_lcms_a;
#endif

		/**
		 * During construction, the order of the weights gets changed.
		 * This mapping allows to later find their original location.
		 * permutation[m] = n
		 * means that a weight now at index m used to be at position n.
		 */
		std::vector<size_type> permutation;

		/**
		 * Collects the decompositions by recursion for the alphabet mass given by @c max_i.
		 * It collects data into @c decompositions,
		 * currently calculating decomposition is given in @c decomposition.
		 *
		 * @param mass Mass to be decomposed.
		 * @param max_i Index that denotes which alphabet mass is used on the current recursion step.
		 * @param decomposition Currently calculating decomposition.
		 * @param decompositions Reference to the store where decompositions are collected.
		 */
		 void decompose(value_type mass_a, value_type mass_c,
				size_type max_i, decomposition_type decomposition,
				decompositions_type& decompositions);
};


/**
 * Constructor for two sets of weights. Decompositions will be done over both
 * weight sets at the same time.
 */
template <typename ValueType, typename DecompositionValueType>
TwoMassDecomposer2<ValueType, DecompositionValueType>::TwoMassDecomposer2(
	const Weights& weights_a_,
	const Weights& weights_b
) :
	weights_a(weights_a_),
	permutation(weights_a.size())
{
	if (weights_a.size() != weights_b.size()) {
		throw InvalidArgumentException("weights_a and weights_b must be of equal size");
	}

	for (size_t i = 0; i < permutation.size(); ++i) {
		permutation[i] = i;
	}

	// combine weights_a and weights_b into a combined weight set.
	// first, find the j for which weights_a[j]/weights_b[j] is minimal
	// (this ensures that the resulting weight set will have only positive
	// weights).
	j = 0;
	for (size_t i = 1; i < weights_a.size(); ++i) {
		// since all weights are positive, we can use the following
		// instead of a[j]/b[j] > a[i]/b[i]
		// cast is necessary to avoid overflow
		if ((unsigned long long)weights_a[j]*weights_b[i] > (unsigned long long)weights_a[i]*weights_b[j]) {
			j = i;
		}
	}

	weights_b_j = weights_b[j];
	// create the new weight set. c_j will already be in the correct position.
	// TODO for now, we create a temporary alphabet first
	Alphabet c;
	// for overflow detection
	Weights::weight_type max_weight = std::numeric_limits<Weights::weight_type>::max();
	for (size_t i = 1; i < weights_a.size(); ++i) {
		unsigned long long weight;
		if (i == j) {
			weight = (unsigned long long)(weights_a[0]) * weights_b[j] - (unsigned long long)(weights_b[0]) * weights_a[j];
		}
		else {
			weight = (unsigned long long)(weights_a[i]) * weights_b[j] - (unsigned long long)(weights_b[i]) * weights_a[j];
		}
		if (weight >= max_weight) {
			throw Exception("overflow while creating combined weight set");
		}
		c.push_back("", weight);
	}
	weights_c = Weights(c.getMasses(), 1.0);

	weights_a.swap(0, j);
	permutation[0] = j;
	permutation[j] = 0;

	// move smallest weight to front (i.e. index 1 in permutation) in weights_c
	size_t min_index = 0;
	for (size_t i = 1; i < weights_c.size(); ++i) {
		if (weights_c[i] < weights_c[min_index]) {
			min_index = i;
		}
	}
	if (min_index != 0) {
		weights_c.swap(0, min_index);
		size_t tmp = permutation[1];
		permutation[1] = permutation[min_index+1];
		permutation[min_index+1] = tmp;
	}

	// create the residue tables for the new weight set weights_c
#ifdef CHECK_A
	lcms_a.resize(weights_a.size());
	mass_in_lcms_a.resize(weights_a.size());

	// create the residue tables for the new weight set weights_c
	rt_a = ResidueTable<ValueType, DecompositionValueType>(weights_a, lcms_a, mass_in_lcms_a, ertable_a);
#endif

	lcms_c.resize(weights_c.size());
	mass_in_lcms_c.resize(weights_c.size());

	rt_c = ResidueTable<ValueType, DecompositionValueType>(weights_c, lcms_c, mass_in_lcms_c, ertable_c);
}


/**
 * Retrieves all decompositions which are
 * - a decomposition of mass_a over weights_a and
 * - a decomposition of mass_b over weights_b
 * simultaneously.
 */
template <typename ValueType, typename DecompositionValueType>
typename TwoMassDecomposer2<ValueType, DecompositionValueType>::decompositions_type
TwoMassDecomposer2<ValueType, DecompositionValueType>::
getAllDecompositions(value_type mass_a, value_type mass_b) {
	// the decompositions found during the recursion are stored here
	decompositions_type decompositions;

	// calculate the correct query mass for the combined weight set
	// Note: The [0] is correct since weights_a[j] was moved to that index.
	long long weight = (long long)mass_a * weights_b_j - (long long)mass_b * weights_a[0];
	if (weight < 0) {
		// negative weight => no decomposition
		return decompositions;
	}
	if (weight >= std::numeric_limits<value_type>::max()) {
		throw Exception("overflow while calculating combined query mass");
	}
	value_type mass_c = weight;

	// the recursion needs an empty witness/decomposition to start
	decomposition_type decomposition(weights_a.size());

	assert(weights_a.size() >= 2);
	// TODO (?) The recursion assumes that it's only called if at least one
	// decomposition over weights_c exists. Ensure this.
	// start recursion
	decompose(mass_a, mass_c, weights_c.size()-1, decomposition, decompositions);

	return decompositions;
}


/**
 * Works only when the first weight set contains exactly one less element
 * than the first.
 * @param decomposition current decomposition. Is "full size"
 * (decomposition.size() == weights_a.size()).
 *
 * remember: weights_a is longer, weights_c are the combined weights
 */
template <typename ValueType, typename DecompositionValueType>
void TwoMassDecomposer2<ValueType, DecompositionValueType>::decompose(
	value_type mass_a,
	value_type mass_c,
	size_type max_i,
	decomposition_type decomposition,
	decompositions_type& decompositions)
{
	// recursion is hitting the top row
	if (max_i == 0) {
		value_type numberOfMasses0 = mass_c / weights_c[0];

		// since we know that the recursion is only called if there actually
		// is a decomposition (over weights_c), we can be sure that the
		// remainder from the above division is zero.
		assert(mass_c % weights_c[0] == 0);

		// for weights_a, there's one more row to go.
		if (numberOfMasses0 * weights_a[1] <= mass_a) {
			mass_a -= numberOfMasses0 * weights_a[1];

			decomposition[permutation[1]] = numberOfMasses0;

			// similar code as above
			value_type n_a = mass_a / weights_a[0];
			// TODO this assertion fails! assert(mass_a % weights_a[0] == 0);
			if (mass_a % weights_a[0] == 0) {
				decomposition[permutation[0]] = n_a;
				decompositions.push_back(decomposition);
			}
		}
		return;
	}

	// cache a few values: this gives a (measured) 15% speed-up
	const value_type lcm = lcms_c[max_i];
	// this is alphabet mass divided by gcd
	const value_type mass_in_lcm_c = mass_in_lcms_c[max_i];
	const value_type mass_mod_decrement = weights_c[max_i] % weights_c[0];
	// change to mass in weight set A
	const value_type dec_mass_a = mass_in_lcm_c * weights_a[max_i+1]; // FIXME

	value_type mass_mod_alphabet0 = mass_c % weights_c[0]; // trying to avoid modulo

#ifdef CHECK_A
	// avoiding modulo in second alphabet
	const value_type dec_residue2 = dec_mass_a % weights_a[0];
#endif

	for (value_type i = 0; i < mass_in_lcm_c; ++i) {
		decomposition[permutation[max_i+1]] = (decomposition_value_type)i;

		// avoid underflow
		if (mass_c < i*weights_c[max_i]) {
			break;
		}
		if (mass_a < i*weights_a[max_i+1]) {
			break;
		}
		// r: current residue class
		const value_type r = ertable_c[max_i-1][mass_mod_alphabet0];

		// TODO if infty was numeric_limits<...>... the following 'if' would not be necessary
		if (r != rt_c.infinity()) {
			assert(max_i+1 < weights_a.size());
			value_type m_a = mass_a - i * weights_a[max_i+1];
#ifdef CHECK_A
			value_type residue2 = m_a % weights_a[0];
#endif

			for (value_type m = mass_c - i * weights_c[max_i]; m >= r; ) {
				/*
				The condition of the 'for' loop (m >= r) and decrementing the mass
				in steps of the lcm ensures that m is decomposable over
				weight set C. If weight set A wasn't there, the recursion would
				always result in at least one witness.
				*/
#ifdef CHECK_A
				value_type r2 = ertable_a[max_i][residue2];
				if (r2 != rt_a.infinity() && m_a >= r2) {
					decompose(m_a, m, max_i-1, decomposition, decompositions);
				}
#else
				decompose(m_a, m, max_i-1, decomposition, decompositions);
#endif
				// avoid underflow
				if (m < lcm || m_a < dec_mass_a) {
					break;
				}

				decomposition[permutation[max_i+1]] += mass_in_lcm_c;
				m -= lcm;
				m_a -= dec_mass_a;

#ifdef CHECK_A
				if (residue2 < dec_residue2) {
					residue2 += weights_a[0] - dec_residue2;
				} else {
					residue2 -= dec_residue2;
				}
#endif
			}
		}
		// subtle way of changing the modulo, instead of calculating it from
		// (mass - i*currentAlphabetMass) % alphabetMass0 every time
		if (mass_mod_alphabet0 < mass_mod_decrement) {
			mass_mod_alphabet0 += weights_c[0] - mass_mod_decrement;
		} else {
			mass_mod_alphabet0 -= mass_mod_decrement;
		}
	}
}

} // namespace ims

#endif // IMS_TWOMASSDECOMPOSER2_H
