#ifndef IMS_TRANSFORMATION_H
#define IMS_TRANSFORMATION_H

#include <cassert>
#include <vector>
#include <iostream>


namespace ims {

/**
 * A transformation. This class is mainly used as a type for return values
 * in functions which calculate a transformation (@see
 * LinearPointSetMatcher.getTransformation() for example) and as parameter
 * type for functions which have to apply an (abstract) transformation to
 * data.
 *
 * This class is abstract, the actual implementation must override transform().
 *
 * @ingroup recalibration
*/
class Transformation {
	public:
		virtual ~Transformation() {}
		/**
		 * Performs the transformation. Should be overridden in derived
		 * classes.
		 */
		virtual double transform(double d) const = 0;
		virtual void print(std::ostream& os) const = 0;
};


/**
 * No-op transformation. Returns the value unchanged.
 */
class IdentityTransformation : public Transformation {
	public:
		virtual double transform(double d) const { return d; }
		virtual void print(std::ostream& os) const;
};


/**
 * Shift transformation. Just adds something to the value.
 *
 * @todo This should be the LinearTransformation, whereas the one below should
 * be called AffineTransformation.
 */
class ShiftTransformation : public Transformation {
	public:
		ShiftTransformation(double translation) : translation(translation) { }
		double getTranslation() const { return translation; }
		virtual double transform(double d) const { return d+translation; }
		virtual void print(std::ostream& os) const;

	private:
		double translation;
};


/**
 * Affine transformation with a scale and translation parameter.
 */
class LinearTransformation : public Transformation {
	public:
		LinearTransformation(double scale, double translation) : scale(scale), translation(translation) { }
		double getTranslation() const { return translation; }
		double getScale() const { return scale; }
		virtual double transform(double d) const { return d*scale+translation; }
		virtual void print(std::ostream& os) const;

	private:
		double scale;
		double translation;
};


/**
 * Polynomial transformation. Returns f(x) where f is a polynomial.
 * Coefficients must be set after construction with setCoefficients().
 */
class PolynomialTransformation : public Transformation {
	public:
		//PolynomialTransformation(std::vector<double> c):coeff(c) {};
		/** Constructor. The coefficients must be set afterwards (they are zero by default). */
		PolynomialTransformation(size_t degree) : degree(degree),  coefficients(degree+1) { }

		/** Sets coefficient i of the polynomial to value.
		 * Remember that degree+1 coefficients should be set
		 * for a full polynomial. Unset coefficients will be zero.
		*/
		void setCoefficient(size_t i, double value) {
			assert(i<=degree);
			coefficients[i] = value;
		} // TODO: throw Exception?
		double getCoefficient(size_t i) const { assert(i<=degree); return coefficients[i]; }
		size_t getDegree() const { return degree; }
		virtual double transform(double d) const;
		virtual void print(std::ostream& os) const;

	private:
		size_t degree;
		std::vector<double> coefficients;
};


/*inline PolynomialTransformation::PolynomialTransformation(double *c, size_t n) : coeff(n+1,0.0) {
	for (size_t i = 0; i <= n; i++) {
		coeff[i] = c[i];
	}
}*/


inline double PolynomialTransformation::transform(double d) const {
	double x = 1.0;
	double result = 0.0;
	for (size_t i = 0; i < coefficients.size(); i++) {
		result += x*coefficients[i];
		x *= d;
	}
	return result;
}

inline std::ostream& operator<< (std::ostream& os, const Transformation& t) {
	t.print(os);
	return os;
}

} // namespace ims

#endif
