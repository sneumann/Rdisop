#ifndef IMS_REALMASSDECOMPOSER_H
#define IMS_REALMASSDECOMPOSER_H

#include <utility>
#include <memory>

#include <ims/decomp/integermassdecomposer.h>

namespace ims {

/**
 * @brief Handles decomposing of non-integer values/masses 
 * over a set of non-integer weights with an error allowed.
 * 
 * Implements a decomposition of non-integer values with a certain error 
 * allowed. Exactness of decomposition can also be tuned by setting a
 * precision factor for weights defining their scaling magnitude.
 * 
 * Works in fact as a wrapper for classes that handle exact mass decomposing
 * using integer arithmetics. Instead of decomposing a single value as done 
 * by integer mass decomposers, @c RealMassDecomposer defines a set of values
 * that lie in the allowed range (defined by error and false negatives 
 * appeared due to rounding), scales those to integers, decomposes
 * them using @c IntegerMassDecomposer, does some checks (i.e. on false 
 * positives appeared due to rounding) and collects decompositions together.
 * 
 * @see IntegerMassDecomposer
 * 
 * @ingroup decomp
 * 
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE> 
 */
class RealMassDecomposer {
	public:
		
		/**
		 * Type of integer decomposer.
		 */
		typedef IntegerMassDecomposer<> integer_decomposer_type;
		
		/**
		 * Type of integer values that are decomposed.
		 */
		typedef integer_decomposer_type::value_type integer_value_type;
		
		/**
		 * Type of result decompositions from integer decomposer.
		 */
		typedef integer_decomposer_type::decompositions_type 
											decompositions_type;

		/**
		 * Type of the number of decompositions.
		 */
		typedef unsigned long long number_of_decompositions_type;

		/**
		 * Constructor with weights.
		 * 
		 * @param weights Weights over which values/masses to be decomposed.
		 */
		RealMassDecomposer(const Weights& weights);
		
		/**
		 * Gets all decompositions for a @c mass with an @c error allowed.
		 * 
		 * @param mass Mass to be decomposed.
		 * @param error Error allowed between given and result decomposition.
		 * @return All possible decompositions for a given mass and error.
		 */
		decompositions_type getDecompositions(double mass, double error);

		/**
		 * Gets a number of all decompositions for a @c mass with an @c error
		 * allowed. It's similar to the @c getDecompositions(double,double) function
		 * but less space consuming, since doesn't use container to store decompositions.
		 * 
		 * @param mass Mass to be decomposed.
		 * @param error Error allowed between given and result decomposition.
		 * @return Number of all decompositions for a given mass and error.
		 */
		number_of_decompositions_type getNumberOfDecompositions(double mass, double error);
	private:
		/**
		 * Weights over which values/masses to be decomposed.
		 */
		Weights weights;
		
		/**
		 * Minimal and maximal rounding errors.
		 */
		std::pair<double, double> rounding_errors;
		
		/**
		 * Precision to scale double values to integer
		 */
		double precision;
		
		/**
		 * Decomposer to be used for exact decomposing using 
		 * integer arithmetics.
		 */
		std::unique_ptr<integer_decomposer_type> decomposer;
};

} // namespace ims

#endif // IMS_REALMASSDECOMPOSER_H
