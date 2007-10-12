/**
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <cmath>
#include <algorithm>
#include <ims/isotopespecies.h>

namespace ims {


IsotopeSpecies& IsotopeSpecies::operator=(const IsotopeSpecies& distribution) {
	if (this != &distribution) {
		isotopes = distribution.isotopes;
	}
	return *this;
}


IsotopeSpecies& IsotopeSpecies::operator*=(const IsotopeSpecies& distribution){
	peaks_container temp_container;
	for (unsigned int i = 0; i < isotopes.size(); i++) {
		for (unsigned int j = 0; j < distribution.size(); j++) {
			temp_container.push_back(peaks_container::value_type(
				isotopes[i].mass + distribution.isotopes[j].mass, 
				isotopes[i].abundance * distribution.isotopes[j].abundance));  
		}
	}
	isotopes.swap(temp_container);
	return *this;
}


IsotopeSpecies& IsotopeSpecies::operator*=(unsigned int power) {
	if (power <= 1) {
		return *this;
	}
	
	// if there is only one isotope
	if (isotopes.size() == 1) {
		// multiplies mass on power and ready
		isotopes[0].mass *= power;
		return *this;
	}

	// sets a new distribution size
	size_type new_size = power + 1;
	if (isotopes.size() > 2) {
		for (size_type i=power+2; i <= power + isotopes.size()-1; ++i) {
			new_size *= i; 
			new_size /= (i-power);
		}
	}		 
	peaks_container temp_container(new_size);

	// sets first element
	mass_type mass0 = power * isotopes[0].mass;
	abundance_type q0 = std::pow(isotopes[0].abundance, static_cast<int>(power));
	temp_container[0] = peaks_container::value_type(
		mass0, q0);	// q(0) = fp^pow, where fp is first isotope probability
		
	if (isotopes.size() == 2) {
		// calculates the binomial distribution		
		// loops and sets all but the first and the last elements.
		for (size_type k = 0; k < new_size-2; ++k) {
			
			// the next mass differs from previous by reducing the one mass of 
			// first isotope, and adding one mass of the second one
			// q(k) = (n-k)/(k+1) * sp/fp * q(k-1), where sp is second isotope probability			
			temp_container[k+1] = peaks_container::value_type(
				temp_container[k].mass - isotopes[0].mass + isotopes[1].mass,   							
				temp_container[k].abundance * (power - k) / (k+1) * isotopes[1].abundance / isotopes[0].abundance);
		}
		
		// sets the last element
		temp_container[new_size-1] = peaks_container::value_type(
			temp_container[new_size-2].mass - isotopes[0].mass + isotopes[1].mass,
			temp_container[new_size-2].abundance / (new_size-1) * isotopes[1].abundance / isotopes[0].abundance);
	} else {
		indexes_container indexes(isotopes.size()-1);
		indexes[0] = 1;
		// first parameter - currently increasing index
		// second - maximum for current indexes
		// third - original power
		// forth - indexes
		updateCoefficients(1, indexes, 0, power, power, temp_container);
	}
	
	isotopes.swap(temp_container);
	
	return *this;
}


void IsotopeSpecies::updateCoefficients(int i, indexes_container& indexes, 
		size_type k, int max_coefficient, int coefficient, peaks_container& temp_container) {
	
	// keeps the counter - how many elements were added
	size_type counter = k;
	
	for (; indexes[0] <= max_coefficient;++indexes[0], ++counter) {
		temp_container[counter+1] = peaks_container::value_type(
			temp_container[counter].mass - isotopes[0].mass + isotopes[1].mass,
			temp_container[counter].abundance * (max_coefficient-indexes[0]+1) / 
				indexes[0] * isotopes[1].abundance / isotopes[0].abundance);
	}
	
	// first finds out the index that can be increased up to @c max	
	// if the end for this coefficient is not reaced
	if (max_coefficient>0) {
		// sets all indexes before to zero, and adds+1 to the found index
		// after that runs the recursion
		updateIndexes(1, indexes);
		
		// updates mass and abundance
		temp_container[++counter] = peaks_container::value_type(
			temp_container[k].mass - isotopes[0].mass + isotopes[2].mass,
			temp_container[k].abundance * max_coefficient / indexes[1] *
				isotopes[2].abundance / isotopes[0].abundance );
								
		// runs recurrence with new parameters
		updateCoefficients(i, indexes, counter, max_coefficient-1, coefficient, temp_container);

	// if the index to be increased is not found, 
	// check whether index @c i has reached the end of isotopes container
	} else  if (coefficient >= indexes[i]+1) {
		updateIndexes(i, indexes);

		// calculates abundance as equation coefficient!/((coefficient-indexes[i])!*indexes[i]!)
		size_type mass = (coefficient >= indexes[i]+1) ? (indexes[i]+1): 1;
		for (size_type j = indexes[i]+2; static_cast<int>(j) <= coefficient; ++j) {
			mass *= j;
			mass /= (j - indexes[i]);
		}
		// updates mass and abundance
		temp_container[++counter] = peaks_container::value_type(
			(coefficient - indexes[i]) * isotopes[0].mass + indexes[i] * isotopes[i+1].mass,
			 mass * std::pow(isotopes[0].abundance, static_cast<int>(coefficient-indexes[i])) * 
			 		std::pow(isotopes[i+1].abundance, static_cast<int>(indexes[i])));

		// runs recurrence with new parameters
		updateCoefficients(i, indexes, counter, coefficient-indexes[i], coefficient, temp_container);
	  } else if ( i+1 < static_cast<int>(indexes.size())) {
		// if the end is not reached, set all indexes to zero(incl. @c i)
		// adds+1 to @c i+1 and run the recursion with updated params
		updateIndexes(i+1, indexes);
		
		// updates mass and abundance
		temp_container[++counter] = peaks_container::value_type(
			(coefficient-1)*isotopes[0].mass + isotopes[i+2].mass,
			coefficient * std::pow(isotopes[0].abundance, static_cast<int>(coefficient)-1) * isotopes[i+2].abundance);
		
		// runs recurrence with new parameters		
		updateCoefficients(i+1, indexes, counter, coefficient-1, coefficient, temp_container);
	}
}


void IsotopeSpecies::updateIndexes(size_type until, indexes_container& indexes){
	indexes[0] = 1;
	for (size_type i = 1; i < until; ++i) {
		indexes[i] = 0;
	}
	++indexes[until];
}


void IsotopeSpecies::sortByMass() {
	std::sort(isotopes.begin(), isotopes.end(), MassSortingCriteria());
}


void IsotopeSpecies::filterAbundances(abundance_type threshold) {
	isotopes.erase(std::remove_if(isotopes.begin(), isotopes.end(),
					AbundanceRemoveCriteria(threshold)),
			isotopes.end());
}


std::ostream& operator<<(std::ostream& os, const IsotopeSpecies& distribution){
	for (IsotopeSpecies::size_type i = 0; i < distribution.size(); ++i) {
		os << distribution.getMass(i) << ' ' << distribution.getAbundance(i) <<'\n';
	}
	return os;
}

} // namespace ims
