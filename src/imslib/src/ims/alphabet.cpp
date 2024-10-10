/**
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <functional>
#include <algorithm>
#include <ims/alphabet.h>
#include <ims/utils/compose_f_gx_t.h>
#include <ims/utils/compose_f_gx_hy_t.h>
#include <ims/base/parser/alphabettextparser.h>


namespace ims {


const Alphabet::name_type& Alphabet::getName(size_type index) const {
	return getElement(index).getName();
}


Alphabet::mass_type Alphabet::getMass(size_type index) const {
	return getElement(index).getMass();
}


Alphabet::mass_type Alphabet::getMass(const name_type& name) const
										/*throw (UnknownCharacterException)*/ {
	return getElement(name).getMass();
}


bool Alphabet::hasName(const name_type& name) const {
        return std::find_if(elements.begin(), elements.end(), [name](const auto& elem) {
	  return std::equal_to<name_type>()(elem.getName(), name);
	}) != elements.end();
}


const Alphabet::element_type& Alphabet::getElement(const name_type& name) const
											/*throw (UnknownCharacterException)*/ {
	const_iterator cit = elements.begin();
	for (; cit != elements.end(); ++cit) {
		if (cit->getName() == name) {
			return *cit;
		}
	}
	throw UnknownCharacterException(name + " was not found in alphabet!");
}


Alphabet::masses_type Alphabet::getMasses(size_type index) const {
	masses_type masses;
	const_iterator cit = elements.begin();
	for (; cit != elements.end(); ++cit) {
		masses.push_back(cit->getMass(index));
	}
	return masses;
}


Alphabet::masses_type Alphabet::getAverageMasses() const {
	masses_type masses;
	const_iterator cit = elements.begin();
	for (; cit != elements.end(); ++cit) {
		masses.push_back(cit->getAverageMass());
	}
	return masses;
}


void Alphabet::sortByNames() {
        std::sort(elements.begin(), elements.end(), [](const auto& lhs, const auto& rhs) {
	  return std::less<name_type>()(lhs.getName(), rhs.getName());
	});
}


void Alphabet::sortByValues() {
	std::sort(elements.begin(), elements.end(), MassSortingCriteria());
}


void Alphabet::load(const std::string& fname) /*throw (IOException)*/ {
	this->load(fname, new AlphabetTextParser);
}


void Alphabet::load(const std::string& fname, AlphabetParser<>* parser)
														/*throw (IOException)*/ {
	parser->load(fname);
	this->clear();
	for (AlphabetParser<>::ContainerType::const_iterator pos =
			parser->getElements().begin(),
			end = parser->getElements().end();	pos != end; ++pos) {
		this->push_back(pos->first, pos->second);
	}
	this->sortByValues();
}


std::ostream& operator <<(std::ostream& os, const Alphabet& alphabet) {
	for (Alphabet::size_type i = 0; i < alphabet.size(); ++i ) {
		os << alphabet.getElement(i) << '\n';
	}
	return os;
}

} // namespace ims
