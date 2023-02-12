/**
 * realmassdecomposer.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */

#include <ims/utils/math.h>
#include <ims/decomp/realmassdecomposer.h>
#include <ims/decomp/decomputils.h>
#include <iostream>

namespace ims {


RealMassDecomposer::RealMassDecomposer(const Weights& weights) :
		weights(weights) {
			
	rounding_errors =
		DecompUtils::getMinMaxWeightsRoundingErrors(weights);
	precision = weights.getPrecision();
	decomposer = std::unique_ptr<integer_decomposer_type>(
							new integer_decomposer_type(weights));
}


RealMassDecomposer::decompositions_type
RealMassDecomposer::getDecompositions(double mass, double error) {
	// defines the range of integers to be decomposed
	integer_value_type start_integer_mass = static_cast<integer_value_type>(
		ceil((1 + rounding_errors.first) * (mass - error) / precision));
	integer_value_type end_integer_mass = static_cast<integer_value_type>(
		floor((1 + rounding_errors.second) * (mass + error) / precision));

	decompositions_type all_decompositions_from_range;

	// loops and finds decompositions for every integer mass,
	// then checks if real mass of decomposition lays in the allowed
	// error interval [mass-error; mass+error]
	for (integer_value_type integer_mass = start_integer_mass;
							integer_mass < end_integer_mass; ++integer_mass) {
		decompositions_type decompositions =
			decomposer->getAllDecompositions(integer_mass);
		for (decompositions_type::iterator pos = decompositions.begin();
			 						pos != decompositions.end();) {
			double parent_mass =
						DecompUtils::getParentMass(weights, *pos);
			if (fabs(parent_mass - mass) > error) {
				pos = decompositions.erase(pos);
			} else {
				++pos;
			}
		}
		all_decompositions_from_range.insert(all_decompositions_from_range.end(), 
								decompositions.begin(), decompositions.end());
	}

	return all_decompositions_from_range;
}

RealMassDecomposer::number_of_decompositions_type
RealMassDecomposer::getNumberOfDecompositions(double mass, double error) {
	// defines the range of integers to be decomposed
	integer_value_type start_integer_mass = static_cast<integer_value_type>(1);
	if (mass - error > 0) {
		start_integer_mass = static_cast<integer_value_type>(
		ceil((1 + rounding_errors.first) * (mass - error) / precision));
	}
	integer_value_type end_integer_mass = static_cast<integer_value_type>(
		floor((1 + rounding_errors.second) * (mass + error) / precision));

	number_of_decompositions_type number_of_decompositions = static_cast<number_of_decompositions_type>(0);

	// loops and finds decompositions for every integer mass,
	// then checks if real mass of decomposition lays in the allowed
	// error interval [mass-error; mass+error]
	for (integer_value_type integer_mass = start_integer_mass;
							integer_mass < end_integer_mass; ++integer_mass) {
		decompositions_type decompositions =
			decomposer->getAllDecompositions(integer_mass);
		for (decompositions_type::iterator pos = decompositions.begin();
			 						pos != decompositions.end(); ++pos) {
			double parent_mass = DecompUtils::getParentMass(weights, *pos);
			if (fabs(parent_mass - mass) <= error) {
				++number_of_decompositions;
			}
		}
	}

	return number_of_decompositions;
}

} // namespace ims
