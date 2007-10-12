/**
 * element.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <ims/element.h>

namespace ims {

/**
 * @note Value for electron mass is taken from 
 * @link www.mcelwee.net/html/table_of_physical_constants.html
 */
const Element::mass_type Element::ELECTRON_MASS_IN_U = 0.00054858;

Element& Element::operator =(const Element& element) {
	// if one doesn't assign object to itself,
	// assign all object elements to the elements of the given object
	if (this != &element) {
		name = element.name;		
		sequence = element.sequence;
		isotopes = element.isotopes;
	}
	return *this;
}


bool Element::operator ==(const Element& element) const {
	return ( this == &element ||
			(name == element.name &&
			 sequence == element.sequence &&
			 isotopes == element.isotopes));
}


bool Element::operator !=(const Element& element) const {
	return !this->operator==(element);
}


std::ostream& operator <<(std::ostream& os, const Element& element) {
	os << "name:\t" << element.getName() << "\nsequence:\t" << element.getSequence()
	   << "\nisotope distribution:\n" << element.getIsotopeDistribution() << '\n';
	return os;
}

} // .. mamespace ims
