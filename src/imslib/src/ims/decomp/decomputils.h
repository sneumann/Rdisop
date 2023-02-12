#ifndef IMS_DECOMPUTILS_H
#define IMS_DECOMPUTILS_H

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <ostream>

#include <ims/utils/print.h>
#include <ims/alphabet.h>

namespace ims {

/**
 * Contains various static utility methods that are used in mass
 * decomposition algorithms.
 *
 * @ingroup decomp
 */
namespace DecompUtils {

template <typename DecompositionType>
std::map<Alphabet::name_type, typename DecompositionType::value_type>*
createCompomer(const Alphabet& alphabet, const DecompositionType& decomposition)
{
	typedef std::map<Alphabet::name_type,
			 typename DecompositionType::value_type> Compomer;
	Compomer* compomer = new Compomer;
	typename DecompositionType::size_type counter = 0;
	for (typename DecompositionType::const_iterator pos = decomposition.begin(); pos != decomposition.end(); ++pos) {
		if (*pos != 0) {
			compomer->insert(typename Compomer::value_type(alphabet.getName(counter), *pos));
		}
		++counter;
	}
	return compomer;
}


template <typename T>
std::vector<T> getNeighborhoodSet(T middle, T range, T granularity) {
	std::vector<T> valuesRange;
	if (range <= 0 || granularity<=0 || range < granularity) {
		valuesRange.push_back(middle);
	} else {
		for (T value = middle - range ; value < middle + range; value += granularity) {
			valuesRange.push_back(value);
		}
	}
	return valuesRange;
}

template <typename T>
std::vector<T> getNeighborhoodSet(T middle, std::pair<T, T> minMaxElements, T range, T granularity) {
//	std::cout << "min="<<minMaxElements.first << ", max=" << minMaxElements.second << ",range="<<range << ",gran="<< granularity << std::endl;
	std::vector<T> valuesRange;
	if (range <= 0 || granularity <= 0 || minMaxElements.first > minMaxElements.second) {
		valuesRange.push_back(middle);
	} else {
		for (T value = minMaxElements.first-range; value < minMaxElements.second+range; value += granularity) {
			valuesRange.push_back(value);
		}
//		PrintingUtils::printOneDimensionalContainer(*valuesRange);
	}
	return valuesRange;
}

template <typename T>
std::vector<T> getPositiveNeighborhoodSet(T middle,
					std::pair<T, T> minMaxElements, T range, T granularity) {
	std::vector<T> valuesRange = getNeighborhoodSet(middle, minMaxElements, range, granularity);
	valuesRange.erase(std::remove_if(valuesRange.begin(), valuesRange.end(), [](const T& x) { return x < 0; }, 0), valuesRange.end());
	return valuesRange;
}

template <typename T>
std::vector<T> getPositiveNeighborhoodSet(T middle, T range, T granularity) {
	std::vector<T> valuesRange = getNeighborhoodSet(middle, range, granularity);
	valuesRange.erase(std::remove_if(valuesRange.begin(), valuesRange.end(), [](const T& x) { return x < 0; }), valuesRange.end());
	return valuesRange;
}


template <typename DecompositionWeights>
std::pair<typename DecompositionWeights::alphabet_mass_type,
typename DecompositionWeights::alphabet_mass_type>
getMinMaxWeightsRoundingErrors(const DecompositionWeights& weights) {

	typedef typename DecompositionWeights::alphabet_mass_type alphabet_mass_type;
	typedef typename DecompositionWeights::weight_type weight_type;
	typedef typename DecompositionWeights::size_type size_type;

	alphabet_mass_type precision = weights.getPrecision(), minNegativeError = 0, maxPositiveError = 0;
	for (size_type i = 0; i < weights.size(); ++i) {
		alphabet_mass_type error = (precision * static_cast<alphabet_mass_type>(weights.getWeight(i))
									- weights.getAlphabetMass(i)) / weights.getAlphabetMass(i);
//		std::cout << "integer weight = " << static_cast<alphabet_mass_type>(weights.getWeight(i))
//				  << ", real mass = " << weights.getAlphabetMass(i)
//				  << ", error = " << error << '\n';
		if (error < 0 && error < minNegativeError) {
			minNegativeError = error;
		} else {
			if (error > 0 && error > maxPositiveError) {
				maxPositiveError = error;
			}
		}
	}
//	std::cout << "minNegativeError = " << minNegativeError << ", maxPositiveError = " << maxPositiveError << '\n';
	return std::make_pair(minNegativeError, maxPositiveError);
}


template <typename MassesType>
std::map<MassesType, std::pair<MassesType, MassesType> > createFalseExcludedRange(const std::vector<MassesType> &masses, const Weights& weights) {
	typedef std::vector<MassesType> MassesContainer;
	typedef std::map<MassesType, std::pair<MassesType, MassesType> > MassesRangeContainer;

	std::pair<MassesType, Alphabet::mass_type> minMaxWeightsError = getMinMaxWeightsRoundingErrors(weights);
	MassesRangeContainer rangeContainer;
	for (typename MassesContainer::const_iterator massesPos = masses.begin(), massesEnd = masses.end(); massesPos != massesEnd; ++massesPos) {
		rangeContainer.insert(typename MassesRangeContainer::value_type(*massesPos,
			std::make_pair(*massesPos * (1+minMaxWeightsError.first), *massesPos * (1+minMaxWeightsError.second))));
	}
	return rangeContainer;
}


template <typename DecompositionWeights, typename DecompositionType>
typename DecompositionWeights::alphabet_mass_type getParentMass(const DecompositionWeights& weights, const DecompositionType& decomposition) {
	typename DecompositionWeights::alphabet_mass_type parentMass = 0;
	typename DecompositionType::size_type counter = 0;
	for (typename DecompositionType::const_iterator pos = decomposition.begin(),
					end = decomposition.end(); pos != end; ++pos) {
		parentMass += (*pos) * weights.getAlphabetMass(counter++);
	}
	return parentMass;
}


template <typename DecompositionWeights, typename DecompositionType>
typename DecompositionWeights::weight_type getIntegerParentMass(const DecompositionWeights& weights, DecompositionType* decomposition) {
	typename DecompositionWeights::weight_type parentMass = 0;
	typename DecompositionType::size_type counter = 0;
	for (typename DecompositionType::const_iterator pos = decomposition->begin();
			pos != decomposition->end(); ++pos) {
		parentMass += (*pos) * weights.getWeight(counter++);
	}
	return parentMass;
}


} // namespace DecompUtils

} // namespace ims

#endif // IMS_DECOMPUTILS_H
