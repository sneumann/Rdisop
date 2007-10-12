#ifndef IMS_TWOMASSDECOMPOSER_H
#define IMS_TWOMASSDECOMPOSER_H

#include <vector>
#include <utility>
#include <cassert>
#include <ims/weights.h>
#include <ims/utils/gcd.h>
#include <ims/decomp/residuetable.h>

namespace ims {
/**
 * The specific implementation of @c MassDecomposer interface using algorithms and data structure 
 * that are described in the paper "Efficient Mass Decomposition" S. Böcker, Zs. Lipták, ACM SAC-BIO, 2004. 
 * The main idea is instead of using the classical dynamic programming algorithm with corresponding data 
 * structures, to store the residues of the smallest decomposable numbers for every modulo of the smallest 
 * alphabet mass. For more detailed description, please, refer to the mentioned paper.
 * @ingroup decomp
 */
template <typename ValueType = long unsigned int,
		  typename DecompositionValueType = unsigned int>
class TwoMassDecomposer {
	public:
		/**
		 * Type of values to be decomposed
		 */
		typedef ValueType value_type;
		
		/**
		 * Type of decomposition values.
		 */
		typedef DecompositionValueType decomposition_value_type;

		/**
		 * Type of output for One Decomposition Problem.
		 */
		typedef std::vector<decomposition_value_type> decomposition_type;

		/**
		 * Type of decomposition size.
		 */
		typedef typename decomposition_type::size_type size_type;

		/**
		 * Type of output for All Decompositions Problem.
		 */
		typedef std::vector<decomposition_type> decompositions_type;

		/**
		 * Constructor with two sets of weights. Decompositions can be done over both
		 * weight sets at the same time.
		 * 
		 * @param weights1 First set of weights over which mass to be decomposed.
		 * @param weights2 Second set of weights over which mass to be decomposed.
		 */
		TwoMassDecomposer(const Weights& weights1, const Weights& weights2);

		/**
		 * Gets all decompositions of @c mass over the first set of weights.
		 * 
		 * @param mass Mass to be decomposed
		 */
		decompositions_type getAllDecompositions(value_type mass);

		/**
		 * Gets all decompositions that simultaneously decompose @c mass1 over the first set of weights,
		 * and @mass2 over the second set of weights.
		 * 
		 * @param mass1 Mass to be decomposed over the first set of weights.
		 * @param mass2 Mass to be decomposed over the second set of weights.
		 */
		decompositions_type getAllDecompositions(value_type mass1, value_type mass2);


	private:
		/**
		 * Type of rows of residues table.
		 */
		typedef std::vector<value_type> row_type;

		/**
		 * Type of the residues table.
		 */
		typedef std::vector<row_type> table_type;

		/**
		 * First set of weights.
		 */
		Weights weights;
		
		/**
		 * Second set of weights.
		 */		
		Weights weights2;

		/**
		 * Table with the residues of the smallest decomposable numbers over
		 * every modulo of the smallest alphabet mass are stored.
		 * Corresponds to the Extended Residue Table in the paper.
		 * Corresponds to the first set of weights.
		 */
		table_type ertable;

		/**
		 * Table with the residues of the smallest decomposable numbers over
		 * every modulo of the smallest alphabet mass are stored.
		 * Corresponds to the Extended Residue Table in the paper.
		 * Corresponds to the second set of weights.
		 */
		table_type ertable2;

		/**
		 * List of the least common multiples. Corresponds to the lcm data structure
		 * in the paper. Corresponds to the first set of weights.
		 */
		row_type lcms;

		/**
		 * List of the least common multiples. Corresponds to the lcm data structure
		 * in the paper. Corresponds to the second set of weights.
		 */
		row_type lcms2;

		/**
		 * List of the counters for the least common multiples that store the number
		 * how often the smallest alphabet mass fits into the correcponding least
		 * common multiple(lcm). Corresponds to the first set of weights.
		 */
		row_type mass_in_lcms;
		
		/**
		 * List of the counters for the least common multiples that store the number
		 * how often the smallest alphabet mass fits into the correcponding least
		 * common multiple(lcm). Corresponds to the second set of weights.
		 */		
		row_type mass_in_lcms2;

		/**
		 * Value that corresponds the infinite value in the paper.
		 * Corresponds to the first set of weights.
		 */
		value_type infty;
		
		/**
		 * Value that corresponds the infinite value in the paper.
		 * Corresponds to the second set of weights.
		 */		
		value_type infty2;

		/**
		 * Collects the decompositions by recursion for the alphabet mass given by @c alphabetMassIndex.
		 * It collects data into @c decompositions,
		 * currently calculating decomposition is given in @c decomposition.
		 *
		 * @param mass Mass to be decomposed.
		 * @param alphabetMassIndex Index that denotes which alphabet mass is used on the current recursion step.
		 * @param decomposition Currently calculating decomposition.
		 * @param &decompositionStore Reference to the store where decompositions are collected.
		 */
		 void collectDecompositionsRecursively(value_type mass, size_type alphabetMassIndex,
				decomposition_type decomposition, decompositions_type& decompositions);

		/**
		 * Collects the decompositions by recursion for the alphabet mass given by @c alphabetMassIndex.
		 * It collects data into @c decompositions,
		 * currently calculating decomposition is given in @c decomposition.
		 *
		 * @param mass1 Mass to be decomposed over the first set of weights.
		 * @param mass2 Mass to be decomposed over the second set of weights.
		 * @param alphabetMassIndex Index that denotes which alphabet mass is used on the current recursion step.
		 * @param decomposition Currently calculating decomposition.
		 * @param &decompositionStore Reference to the store where decompositions are collected.
		 */
		 void collectDecompositionsRecursively(value_type mass1, value_type mass2,
				size_type alphabetMassIndex, decomposition_type decomposition,
				decompositions_type& decompositions);
};


/**
 * Constructor with two sets of weights. Decompositions can be done over both
 * weight sets at the same time.
 */
template <typename ValueType, typename DecompositionValueType>
TwoMassDecomposer<ValueType, DecompositionValueType>::TwoMassDecomposer(
	const Weights& weights,
	const Weights& weights2
) :
	weights(weights),
	weights2(weights2) {
		
	lcms.reserve(weights.size());
	lcms.resize(weights.size());

	mass_in_lcms.reserve(weights.size());
	mass_in_lcms.resize(weights.size());

	ResidueTable<ValueType, DecompositionValueType> rt(weights, lcms, mass_in_lcms, ertable);
	infty = rt.infinity();

	lcms2.reserve(weights2.size());
	lcms2.resize(weights2.size());

	mass_in_lcms2.reserve(weights2.size());
	mass_in_lcms2.resize(weights2.size());

	ResidueTable<ValueType, DecompositionValueType> rt2(weights2, lcms2, mass_in_lcms2, ertable2);
	infty2 = rt2.infinity();
}


/**
 * Gets all decompositions for @c mass over the first set of weights.
 */
template <typename ValueType, typename DecompositionValueType>
typename TwoMassDecomposer<ValueType, DecompositionValueType>::decompositions_type
TwoMassDecomposer<ValueType, DecompositionValueType>::getAllDecompositions(value_type mass) {
	// initialize the decompositions store and value for storing current decomposition
	decompositions_type decompositions;
	decomposition_type decomposition(weights.size());

	// run the recursive algorithm to collect decompositions
	collectDecompositionsRecursively(mass, weights.size()-1, decomposition, decompositions);

	return decompositions;
}


/**
 * Gets all decompositions that simultaneously decompose @c mass1 over the first set of weights,
 * and @mass2 over the second set of weights.
 */
template <typename ValueType, typename DecompositionValueType>
typename TwoMassDecomposer<ValueType, DecompositionValueType>::decompositions_type
TwoMassDecomposer<ValueType, DecompositionValueType>::
getAllDecompositions(value_type mass1, value_type mass2) {
	// initialize the decompositions store and value for storing current decomposition
	decompositions_type decompositions;

	decomposition_type decomposition(weights.size());
	collectDecompositionsRecursively(mass1, mass2, weights.size()-1, decomposition, decompositions);

	return decompositions;
}


/**
 * Collects the decompositions by recursion for the alphabet mass given by @c alphabetMassIndex.
 * It collects data into @c decompositions,
 * currently calculating decomposition is given in @c decomposition.
 *
 * @param mass Mass to be decomposed.
 * @param alphabetMassIndex Index that denotes which alphabet mass is used on the current recursion step.
 * @param decomposition Currently calculating decomposition.
 * @param &decompositionStore Reference to the store where decompositions are collected.
 */
template <typename ValueType, typename DecompositionValueType>
void TwoMassDecomposer<ValueType, DecompositionValueType>::collectDecompositionsRecursively(
	value_type mass,
	size_type alphabetMassIndex,
	decomposition_type decomposition,
	decompositions_type& decompositions) {
		
	if (alphabetMassIndex == 0) {
		value_type numberOfMasses0 = mass / weights.getWeight(0);
		if (numberOfMasses0 * weights.getWeight(0) == mass) {
			// TODO: here is the conversion from value_type to decomposition_value_type
			decomposition[0] = (decomposition_value_type)numberOfMasses0;
			decompositions.push_back(decomposition);
		}
		return;
	}

	// cache a few values: this gives a 15% speed-up
	const value_type lcm = lcms[alphabetMassIndex];
	const value_type mass_in_lcm = mass_in_lcms[alphabetMassIndex]; // this is alphabet mass divided by gcd
	const value_type mass_mod_decrement = weights.getWeight(alphabetMassIndex) % weights.getWeight(0);

	value_type mass_mod_alphabet0 = mass % weights.getWeight(0); // trying to avoid modulo

	for (value_type i = 0; i < mass_in_lcm; ++i) {
		// here is the conversion from value_type to decomposition_value_type
		decomposition[alphabetMassIndex] = (decomposition_value_type)i;

		// this check is needed because mass could have unsigned type and after reduction on i*alphabetMass will be still be positive but huge
		// and that will end up in unfinite loop
		if (mass < i*weights.getWeight(alphabetMassIndex)) {
			break;
		}

		// current residue class
		const value_type r = ertable[alphabetMassIndex-1][mass_mod_alphabet0];

		// TODO: if infty was std::numeric_limits<...>... the following 'if' would not be necessary
		if (r != infty) {
			for (value_type m = mass - i * weights.getWeight(alphabetMassIndex); m >= r; m -= lcm) {
				/* the condition of the 'for' loop (m >= r) and decrementing the mass
				 * in steps of the lcm ensure that m is decomposable. Therefore
				 * the recursion will result in at least one witness. */
				collectDecompositionsRecursively(m, alphabetMassIndex-1, decomposition, decompositions);
				decomposition[alphabetMassIndex] += mass_in_lcm;
				// avoid underflow in next iteration
				if (m < lcm) {
					break;
				}
			}
		}
		// subtle way of changing the modulo, instead of plain calculation it from
		// (mass - i*currentAlphabetMass) % alphabetMass0 every time
		if (mass_mod_alphabet0 < mass_mod_decrement) {
			mass_mod_alphabet0 += weights.getWeight(0) - mass_mod_decrement;
		} else {
			mass_mod_alphabet0 -= mass_mod_decrement;
		}
	}

}


/**
 * Collects the decompositions by recursion for the alphabet mass given by @c alphabetMassIndex.
 * It collects data into @c decompositions,
 * currently calculating decomposition is given in @c decomposition.
 *
 * @param mass1 Mass to be decomposed over the first set of weights.
 * @param mass2 Mass to be decomposed over the second set of weights.
 * @param alphabetMassIndex Index that denotes which alphabet mass is used on the current recursion step.
 * @param decomposition Currently calculating decomposition.
 * @param &decompositions Reference to the store where decompositions are collected.
 */
template <typename ValueType, typename DecompositionValueType>
void TwoMassDecomposer<ValueType, DecompositionValueType>::
collectDecompositionsRecursively(
	value_type mass1,
	value_type mass2,
	size_type alphabetMassIndex,
	decomposition_type decomposition,
	decompositions_type& decompositions) {
		
	if (alphabetMassIndex == 0) {
		value_type numberOfMasses0 = mass1 / weights.getWeight(0);
		value_type numberOfMasses0_mass2 = mass2 / weights2.getWeight(0);

		if (numberOfMasses0 * weights.getWeight(0) == mass1 &&
		    numberOfMasses0 == numberOfMasses0_mass2) {
			// TODO: here is the conversion from value_type to decomposition_value_type
			decomposition[0] = (decomposition_value_type)numberOfMasses0;
			decompositions.push_back(decomposition);
		}
		return;
	}

	const value_type lcm = lcms[alphabetMassIndex];
	const value_type mass_in_lcm = mass_in_lcms[alphabetMassIndex]; // this is alphabet mass divided by gcd
	const value_type mass_mod_decrement = weights.getWeight(alphabetMassIndex) % weights.getWeight(0);
	// change to mass in second alphabet
	const value_type dec_mass2 = mass_in_lcm * weights2.getWeight(alphabetMassIndex);

	value_type mass_mod_alphabet0 = mass1 % weights.getWeight(0); // trying to avoid modulo

	// avoiding modulo in second alphabet
	const value_type dec_residue2 = dec_mass2 % weights2.getWeight(0);

	for (value_type i = 0; i < mass_in_lcm; ++i) {
		// concerns mass 1
		// here is the conversion from value_type to decomposition_value_type
		decomposition[alphabetMassIndex] = (decomposition_value_type)i;

		// avoid underflow (?)
		//this check is needed because mass could have unsigned type and after reduction on i*alphabetMass will be still be positive but huge
		// and that will end up in infinite loop
		if (mass1 < i*weights.getWeight(alphabetMassIndex)) {
			break;
		}
		// NEW
		if (mass2 < i*weights2.getWeight(alphabetMassIndex)) {
			break;
		}
		// current residue class
		const value_type r = ertable[alphabetMassIndex-1][mass_mod_alphabet0]; // "lbound" in paper

		// TODO: if infty was std::numeric_limits<...>... the following 'if' would not be necessary
		if (r != infty) {
			value_type m2 = mass2 - i * weights2.getWeight(alphabetMassIndex);
			value_type residue2 = m2 % weights2.getWeight(0);

			for (value_type m1 = mass1 - i * weights.getWeight(alphabetMassIndex); m1 >= r; ) {
				/* the condition of the 'for' loop (m >= r) and decrementing the mass
				 * in steps of the lcm ensures that m is decomposable. Therefore
				 * the recursion will result in at least one witness. */

				value_type r2 = ertable2[alphabetMassIndex-1][residue2];

				if (r2 != infty2 && m2 >= r2) {
					collectDecompositionsRecursively(m1, m2, alphabetMassIndex-1,
					decomposition, decompositions);
				}

				// avoid underflow
				if (m1 < lcm || m2 < dec_mass2) {
					break;
				}

				decomposition[alphabetMassIndex] += mass_in_lcm;
				m1 -= lcm;
				m2 -= dec_mass2;

				if (residue2 < dec_residue2) {
					residue2 += weights2.getWeight(0) - dec_residue2;
				} else {
					residue2 -= dec_residue2;
				}
			}
		}
		// subtle way of changing the modulo, instead of calculating it from
		// (mass - i*currentAlphabetMass) % alphabetMass0 every time
		if (mass_mod_alphabet0 < mass_mod_decrement) {
			mass_mod_alphabet0 += weights.getWeight(0) - mass_mod_decrement;
		} else {
			mass_mod_alphabet0 -= mass_mod_decrement;
		}
	}

}

} // namespace ims

#endif // IMS_TWOMASSDECOMPOSER_H
