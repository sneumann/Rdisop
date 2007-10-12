#include <iomanip>

#include <ims/transformation.h>

namespace ims {

void IdentityTransformation::print(std::ostream& os) const
{
	os << "f(x) = x";
}

void ShiftTransformation::print(std::ostream& os) const
{
	double translation = getTranslation();
	os << "f(x) = x " << ((translation >= 0.0) ? "+" : "") << translation;
}


void LinearTransformation::print(std::ostream& os) const
{
	double translation = getTranslation();
	os << "f(x) = " << getScale() << "*x " << ((translation >= 0.0) ? "+" : "") << translation;
}


void PolynomialTransformation::print(std::ostream& os) const
{
	bool first = true;
	size_t degree = getDegree();
	for (size_t i = degree+1; i >= 2; --i) {
		if (first) {
			double scale = getCoefficient(i);
			os << scale << " * x**" << i;
			first = false;
		}
		else {
			os << " " << getCoefficient(i) << " * x**" << i;
		}
	}
	if (degree >= 1) {
		double c = getCoefficient(1);
		if (first) {
			os << (c >= 0.0 ? "+" : "" ) << " * x";
		}
		else {
			os << (c >= 0.0 ? " +" : " " ) << " * x";
		}
	}
	if (first) {
		os << getCoefficient(0);
	}
	else {
		os << " " << getCoefficient(0);
	}
}

} // namespace ims
