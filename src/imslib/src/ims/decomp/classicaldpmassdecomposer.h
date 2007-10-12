#ifndef IMS_CLASSICALDPMASSDECOMPOSER_H
#define IMS_CLASSICALDPMASSDECOMPOSER_H

#include <vector>
#include <limits>
#include <ims/decomp/massdecomposer.h>
#include <ims/weights.h>
#include <ims/utils/math.h>
#include <ims/base/exception/exception.h>

namespace ims {

/**
 * @brief Implements @c MassDecomposer interface using classical dynamic 
 * programming algorithm.
 * 
 * Solving decomposition problems is divided in two phases: 
 *   - table (cash) construction 
 *   - backtracking algorithm over the constructed table.
 * 
 * Classical implementation has the following time runtimes for table 
 * construction and backtracking algorithm:
 *
 * Construction time for all problems is O(kM) where @c k is the size 
 * of the alphabet, and @c M is the mass to be decomposed;
 *
 * Existence and Number of decompositions problems:
 *	don't require backtracking algorithm;
 *  result is obtained by direct cache access in a constant time.
 *
 * One Decomposition problem:
 *  backtracking time is in worst case @c O(M/a[1]), 
 * 							where @c a[1] is the smallest mass of the alphabet.
 *
 * All Decompositions problem:
 *  backtracking time is O(N*M/a[1]), 
 * 							where N - is a number of decompositions.
 *
 * @param ValueType Type of values to be decomposed.
 * @param DecompositionValueType Type of decomposition elements.
 * 
 * @see IntegerMassDecomposer
 * 
 * @ingroup decomp
 * 
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>  
 * @author Marcel Martin <Marcel.Martin@CeBiTec.Uni-Bielefeld.DE>
 * @author Henner Sudek <Henner.Sudek@CeBiTec.Uni-Bielefeld.DE> 
 */
template <typename ValueType = long unsigned int,
          typename DecompositionValueType = unsigned int>
class ClassicalDPMassDecomposer : 
					public MassDecomposer<ValueType, DecompositionValueType> {
	public:
		/**
		 * Type of value to be decomposed.
		 */
		typedef typename MassDecomposer<ValueType,DecompositionValueType>::
			value_type value_type;

		/**
		 * Type of decomposition value.
		 */
		typedef typename MassDecomposer<ValueType,DecompositionValueType>::
			decomposition_value_type decomposition_value_type;

		/**
		 * Type of decomposition.
		 */
		typedef typename MassDecomposer<ValueType,DecompositionValueType>::
			decomposition_type decomposition_type;

		/**
		 * Type of container for many decompositions.
		 */
		typedef typename MassDecomposer<ValueType,DecompositionValueType>::
			decompositions_type decompositions_type;
	
		/**
		 * Type of decomposition's size.
		 */
		typedef typename decomposition_type::size_type size_type;

		/**
		 * A default constructor.
		 *
		 * @param weights Set of weights over which masses to be decomposed.
		 */
		ClassicalDPMassDecomposer(const Weights& weights);

		/**
		 * A destructor.
		 */
		virtual ~ClassicalDPMassDecomposer(){}

		/**
		 * @see MassDecomposer::exist(value_type)
		 */
		virtual bool exist(value_type mass);

		/**
		 * @see MassDecomposer::getDecomposition(value_type)
		 */
		virtual decomposition_type getDecomposition(value_type mass);

		/**
		 * @see MassDecomposer::getAllDecompositions(value_type)
		 */
		virtual decompositions_type getAllDecompositions(value_type mass);

		/**
		 * @see MassDecomposer::getNumberOfDecompositions(value_type)
		 */
		virtual decomposition_value_type getNumberOfDecompositions(
													value_type mass);

	private:
		/**
		 * Type of container to store DP table for existence problem
		 */
		typedef typename std::vector<bool> existence_table_type;
		
		/**
		 * Index type of container to store DP table for existence problem
		 */
		typedef typename existence_table_type::size_type 
											existence_table_size_type;
				
		/**
		 * Weights over which the mass is to be decomposed.
		 */
		Weights weights;

		/**
		 * The two-dimensional table which is used as a cache in 
		 * 'all decompositions' and 'number of decompositions' problems.
		 * The reason to use the cache is to improve performance 
		 * so that the useful data is not calculated every time for 
		 * a new value, but instead it's kept in cache
		 * and new data is added only in case it wasn't calculated before.
		 */
		 decompositions_type decompositionsTable;

		/**
		 * The one-dimensional boolean table which is used as a cache in
		 * 'existence' and 'one decomposition' problems.
		 * The reason to use the cache is to improve performance 
		 * so that the useful data is not calculated every time for 
		 * a new value, but instead it's kept in cache
		 *  and new data is added only in case it wasn't calculated before.
		 */
		existence_table_type existenceTable;

		/**
		 * Fills the one-dimensional table up to the given @c value.
		 *
		 * @param value Value up to which 
		 * 					one-dimensional table is to be filled.
		 */
		void fillExistenceTable(existence_table_size_type value);

		/**
		 * Fills the two-dimensional table up to the given @c value.
		 *
		 * @param value Value up to which
		 * 					two-dimensional table is to be filled.
		 */
		void fillDecompositionsTable(size_type value);

		/**
		 * Fills the two-dimensional table in the range 
		 * from @c firstColumn to @c lastColumn.
		 *
		 * @param firstColumn The first column from which 
		 * 								table is to be filled.
		 * @param lastColumn The last column up to which 
		 * 								table is to be filled.
		 */
		void fillDecompositionsTable(size_type firstColumn, 
									 size_type lastColumn);

		/**
		 * Returns all decompositions for the given @c mass. 
		 * Uses backtracking algorithm over the two-dimensional table.
		 *
		 * @param mass Mass to be decomposed.
		 * @return All possible decomposition for a given mass.
		 */
		decompositions_type getDecompositionsByBacktrackingAlgorithm(
														value_type mass);

		/**
		 * Collects decompositions for @c mass by recursion. 
		 *
		 * @param mass Mass to be decomposed.
		 * @param alphabetMassIndex An index of the mass in alphabet 
		 * 							that is used on this step of recursion.
		 * @param decomposition Decomposition which is calculated 
		 * 										 on this step of recursion.
		 * @param decompositionStore Container where 
		 * 										 decompositions are collected.
		 */
		void collectDecompositionsRecursively(value_type mass, 
			size_type alphabetMassIndex, decomposition_type decomposition,
			decompositions_type& decompositionsStore);

};


template <typename ValueType, typename DecompositionValueType>
ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
ClassicalDPMassDecomposer(const Weights& weights): weights(weights) {
	if (weights.getWeight(0) > std::numeric_limits
							<existence_table_size_type>::max() || 
		weights.getWeight(0) > std::numeric_limits
							<size_type>::max()) {
		throw Exception("Decomposition Error: mass is too big to be decomposed!	Mass exceeds numeric limits for cache size.");
	}
	// reserves the space and initializes caches.
	decompositionsTable.reserve(weights.size());
	existenceTable.reserve(weights.getWeight(0));
	existenceTable.assign(weights.getWeight(0), false);
	existenceTable[0] = true;
}


template <typename ValueType, typename DecompositionValueType>
bool ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
exist(value_type mass) {
	if (mass > std::numeric_limits<existence_table_size_type>::max()) {
		throw Exception("Decomposition Error: mass is too big to be decomposed! Mass exceeds numeric limits for cache size.");
	}
	// fills the internal cache up to the given mass
	fillExistenceTable(static_cast<existence_table_size_type>(mass));
	// and returns the value of the last row and the mass as the column
	// which corresponds the boolean value if there are any decompositions
	// of the given mass exist.
	return existenceTable.at(mass);
}


template <typename ValueType, typename DecompositionValueType>
void ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
fillExistenceTable(existence_table_size_type value) {
	// if data for value is already in cache: nothing to be done
	if (existenceTable.size() > value) {
		// cache hit: nothing to be done
		return;
	}
	// prepare data to fill
	existence_table_size_type firstElementToFill = existenceTable.size();
	existenceTable.resize(value+1);

	// table(cache) construction:
	// the algorithm is following:
	// A[m] is true, if exists A[m-a[i]] that is true too, 
	// 						for i=1..k, where k - size of alphabet;
	// 		is false, otherwise.
	for(existence_table_size_type elementToFill = firstElementToFill; 
							elementToFill < value+1; ++elementToFill) {
		existenceTable[elementToFill] = false;
		size_type alphabetMassIndex = 0;
		while (!existenceTable[elementToFill] && 
				alphabetMassIndex < weights.size() ) {
			if (elementToFill >= weights.getWeight(alphabetMassIndex)) {
				existenceTable[elementToFill] = existenceTable[
						elementToFill-weights.getWeight(alphabetMassIndex)];
				++alphabetMassIndex;
			} else {
				break;
			}
		}
	}
}


template <typename ValueType, typename DecompositionValueType>
typename ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
decomposition_type ClassicalDPMassDecomposer<ValueType, 
DecompositionValueType>::getDecomposition(value_type mass) {
	if (mass > std::numeric_limits<existence_table_size_type>::max()) {
		throw Exception("Decomposition Error: mass is too big to be decomposed! Mass exceeds numeric limits for cache size.");
	}

	fillExistenceTable(static_cast<existence_table_size_type>(mass));

	decomposition_type decomposition;

	// if no decomposition found (what corresponds the value 'false'
	// in the one-dimensional table with mass as an index),
	// returns empty vector
	if(!existenceTable[mass]){
		return decomposition;
	}

	// prepares data for usage
	decomposition.reserve(weights.size());
	decomposition.resize(weights.size());

	// runs backtracking algorithm:
	// for every alphabet mass, it's calculated how many of them 
	// has the given mass
	for (size_type alphabetMassIndex = weights.size()-1; 
		 alphabetMassIndex > 0 && mass > 0; --alphabetMassIndex){
		value_type alphabetMass = weights.getWeight(alphabetMassIndex);
		decomposition_value_type numberOfMasses = 0;
		while (mass >= alphabetMass && existenceTable[
					static_cast<existence_table_size_type>(
											mass - alphabetMass)]){
			mass -= alphabetMass;
			++numberOfMasses;
		}
		decomposition[alphabetMassIndex] = numberOfMasses;
	}

	decomposition[0] = mass / weights.getWeight(0);
	return decomposition;
}


template <typename ValueType, typename DecompositionValueType>
void ClassicalDPMassDecomposer<ValueType, DecompositionValueType>
::fillDecompositionsTable(size_type value) {
	// Three possibilites:
	// - first: decompositionsTable is uninitialized	
	if (decompositionsTable.size() == 0) {
		decompositionsTable.assign(weights.size(),decomposition_type(value+1));
		fillDecompositionsTable(0, value);
	// - second: cache miss			
	} else if (decompositionsTable.front().size() < value+1) {
		size_type startColumn = decompositionsTable.front().size();
		for (typename decompositions_type::iterator 
				begin = decompositionsTable.begin(),
				end = decompositionsTable.end(); begin != end; ++begin) {
			begin->resize(value+1);
		}
		fillDecompositionsTable(startColumn, value);
	// - third: cache hit		
	} else { 
		// nothing to be done...
	}
}


template <typename ValueType, typename DecompositionValueType>
void ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
	fillDecompositionsTable(size_type firstColumn, size_type lastColumn) {

	// temporary variable for alphabet masses
	size_type alphabetMass = static_cast<size_type>(weights.getWeight(0));
	// fills first row
	// starts index ensures that filling starts from the next
	// multiplier on 'weights.getWeight(0)' closest to 'firstColumn'
	for (size_type j =  ((firstColumn-1 + alphabetMass) / alphabetMass)
						 * alphabetMass;
					j <= lastColumn; j+= alphabetMass) {
		decompositionsTable[0][j] = 1;
	}

	// fills remaining rows
	for (size_type i = 1; i < weights.size(); ++i) {
		alphabetMass = static_cast<size_type>(weights.getWeight(i));
		// The first values until i-th alphabet mass can be copied from the
		// row above because they don't have a 'partner' at index 'j-mass'.
		for (size_type j = firstColumn; j < alphabetMass && j <= lastColumn;
																		 ++j) {
			decompositionsTable[i][j] = decompositionsTable[i-1][j];
		}
		// the remaining values are filled on the basis of classical algorithm
		// of dynamic programming
		for (size_type j = alphabetMass; j <= lastColumn; ++j) {
			decompositionsTable[i][j] = decompositionsTable[i-1][j]
				+ decompositionsTable[i][j-alphabetMass];
		}
	}
}


template <typename ValueType, typename DecompositionValueType>
typename ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
decomposition_value_type ClassicalDPMassDecomposer<ValueType, 
DecompositionValueType>::getNumberOfDecompositions(value_type mass) {

	if (mass > std::numeric_limits<size_type>::max()) {
		throw Exception("Decomposition Error: mass is too big to be decomposed! Mass exceeds numeric limits for cache size.");
	}
	fillDecompositionsTable(mass);
	return decompositionsTable.back().at(mass);
}


template <typename ValueType, typename DecompositionValueType>
typename ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
decompositions_type ClassicalDPMassDecomposer<ValueType, 
DecompositionValueType>::getAllDecompositions(value_type mass) {

	if (mass > std::numeric_limits<size_type>::max()) {
		throw Exception("Decomposition Error: mass is too big to be decomposed! Mass exceeds numeric limits for cache size.");
	}
	fillDecompositionsTable(static_cast<size_type>(mass));	
	return getDecompositionsByBacktrackingAlgorithm(mass);
}


template <typename ValueType, typename DecompositionValueType>
typename ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
decompositions_type ClassicalDPMassDecomposer<ValueType, 
DecompositionValueType>::getDecompositionsByBacktrackingAlgorithm(
value_type mass) {
	// initializes decompositions store and variable for storing 
	// current decomposition
	decompositions_type decompositionsStore;
	decomposition_type decomposition(weights.size());
	// runs the recursive algorithm to collect decompositions
	collectDecompositionsRecursively(mass, weights.size()-1, 
								decomposition, decompositionsStore);
	return decompositionsStore;
}


template <typename ValueType, typename DecompositionValueType>
void ClassicalDPMassDecomposer<ValueType, DecompositionValueType>::
collectDecompositionsRecursively(value_type mass, size_type 
			alphabetMassIndex, decomposition_type decomposition,
			decompositions_type& decompositionsStore) {

	// if the recursion reachs the first alphabet mass
	if (alphabetMassIndex == 0) {
		// calculates how much of them is in remained mass
		decomposition[0] = mass/weights.getWeight(0);
		// adds the final decomposition to store and return
		decompositionsStore.push_back(decomposition);
		return;
	} else {
		while (mass > 0) {
			// first, checks the previous element to the mass in the column,
			// if it's not zero,
			if (decompositionsTable[alphabetMassIndex-1][mass]) {
				// calls recursion for this previous element.
				collectDecompositionsRecursively(mass, alphabetMassIndex-1, 
										decomposition, decompositionsStore);
			}
			value_type currentAlphabetMass = 
								weights.getWeight(alphabetMassIndex);
			// second, calculates is there any current alphabet 
			// masses in a remained mass
			if (mass >= currentAlphabetMass && decompositionsTable[
					alphabetMassIndex][static_cast<size_type>
											(mass-currentAlphabetMass)]) {
				++decomposition[alphabetMassIndex];
				mass -= currentAlphabetMass;
				// if the mass has been decreased to zero,
				// pushs the current decomposition and return.
				if (mass == 0) {
					decompositionsStore.push_back(decomposition);
					return;
				}
			} else {
				return;
			}
		} // .. while
	} // .. else

}

} // ..namespace ims

#endif //IMS_CLASSICALDPMASSDECOMPOSER_H
