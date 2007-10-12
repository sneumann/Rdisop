/**
 * @author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <ims/utils/math.h>
#include <ims/utils/gcd.h>
#include <ims/weights.h>

namespace ims {

Weights::Weights(const alphabet_masses_type& masses, alphabet_mass_type precision) :
													alphabet_masses(masses),
													precision(precision) {
	setPrecision(precision);
}

Weights& Weights::operator =(const Weights& other) {
	if (this != &other) {
		alphabet_masses = other.alphabet_masses;
		precision = other.precision;
		weights = other.weights;
	}
	return *this;
}


void Weights::setPrecision(Weights::alphabet_mass_type precision) {
	this->precision = precision;
	weights.clear();
	for (alphabet_masses_type::size_type i = 0;
										 i < alphabet_masses.size(); ++i) {
		weights.push_back(static_cast<weight_type>(round(alphabet_masses[i]
																/ precision)));
	}
}


void Weights::swap(size_type index1, size_type index2) {
	weight_type weight = weights[index1];
	weights[index1] = weights[index2];
	weights[index2] = weight;

	alphabet_mass_type mass = alphabet_masses[index1];
	alphabet_masses[index1] = alphabet_masses[index2];
	alphabet_masses[index2] = mass;
}


/**
 * Divides the integer weights by their gcd. The precision is also adjusted.
 *
 * For example, given alphabet weights 3.0, 5.0, 8.0 with precision 0.1, the
 * integer weights would be 30, 50, 80. After calling this method, the new
 * weights are 3, 5, 8 with precision 1.0 (since the gcd of 30, 50, and 80
 * is 10).
 *
 * @return true if anything was changed, that is, if the gcd was &gt; 1.
 *         false if the gcd was already 1 or there are less than two weights.
*/
bool Weights::divideByGCD() {
	if (weights.size() < 2) {
		return false;
	}
	weight_type d = gcd(weights[0], weights[1]);
	for (weights_type::size_type i = 2; i < weights.size(); ++i) {
		d = gcd(d, weights[i]);
		if (d == 1) {
			return false;
		}
	}
	// if we're here: d != 1

	precision *= d;

	// rescales the integer weights. Don't use setPrecision() here since
	// the result could be different due to rounding errors.
	for (weights_type::size_type i = 0; i < weights.size(); ++i) {
		weights[i] /= d;
	}
	return true;
}


std::ostream& operator<<(std::ostream& os, const Weights& weights) {
	for (Weights::size_type i = 0; i < weights.size(); ++i ) {
		os << weights.getWeight(i) << std::endl;
	}
	return os;
}


} // namespace ims
