/**
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <vector>
#include <algorithm>
#include <sstream>
#include <ostream>
#include <ims/composedelement.h>
#include <ims/base/parser/moleculesequenceparser.h>
#include <ims/base/parser/standardmoleculesequenceparser.h>
#include <iostream>

namespace ims {


ComposedElement::ComposedElement(const container& elements, 
		const std::vector<name_type>* sequence_order) :	elements(elements) {
	this->updateSequence(sequence_order);
	this->updateIsotopeDistribution();
}


ComposedElement::ComposedElement(const name_type& sequence, const Alphabet& alphabet, unsigned sequence_type) 
		/*throw (UnknownCharacterException)*/ {
	this->setSequence(sequence);
	std::unique_ptr<sequence_parser_type> parser;
	if (sequence_type == TEX_NOTATION_MOLECULE_SEQUENCE_TYPE) {
		parser = std::unique_ptr<sequence_parser_type>(new StandardMoleculeSequenceParser);
	} else {
		parser = std::unique_ptr<sequence_parser_type>(new MoleculeSequenceParser);
	}
	this->initializeElements(alphabet, std::move(parser) );
}
			
ComposedElement::ComposedElement(const std::vector<unsigned int>& decomposition, 
			const Alphabet& alphabet) {
	this->initializeElements(decomposition, alphabet);
}

ComposedElement& ComposedElement::operator=(const ComposedElement& element) {
	if (this != &element) {
		elements = element.elements;
		this->setName(element.getName());
		this->setSequence(element.getSequence());
	}
	return *this;
}


bool ComposedElement::operator ==(const ComposedElement& element) const {
	return ( this == &element || 
			(elements == element.elements &&
			 this->getName() == element.getName()) );	
}


bool ComposedElement::operator !=(const ComposedElement& element) const {
	return !this->operator==(element);
}

ComposedElement& ComposedElement::operator -=(const ComposedElement& element) {
	container substracts = element.getElements();
	for (container::const_iterator it = substracts.begin();
		it != substracts.end(); ++it) {
		name_type name = it->first.getName();
		container::iterator itt = std::find_if(
		 	elements.begin(), elements.end(), FindElementByName(name));
		if (itt != elements.end()) {
			if (itt->second > it->second) {
				elements[itt->first] = itt->second - it->second;
			} else {
				elements.erase(itt);
			}
		}
	}
	this->updateSequence();
	this->updateIsotopeDistribution();

	return *this;
}


ComposedElement& ComposedElement::operator +=(const ComposedElement& element) {
	container adducts = element.getElements();
	for (container::const_iterator it = adducts.begin();
		it != adducts.end(); ++it) {
		name_type name = it->first.getName();
		container::iterator itt = std::find_if(
		 	elements.begin(), elements.end(), FindElementByName(name));
		if (itt != elements.end()) {
			elements[itt->first] = itt->second + it->second;
		} else {
			elements[it->first] = it->second;
		}
	}
	this->updateSequence();
	this->updateIsotopeDistribution();

	return *this;
}


ComposedElement::container::mapped_type 
ComposedElement::getElementAbundance(const name_type& name) const {

	container::mapped_type abundance = 0;	
	container::const_iterator element_it = std::find_if(
	 	elements.begin(), elements.end(), FindElementByName(name));
	if (element_it != elements.end()) {
		abundance = element_it->second;
	}
	return abundance;
}


void ComposedElement::initializeElements(const Alphabet& alphabet, std::unique_ptr<sequence_parser_type> parser)
			/*throw (UnknownCharacterException)*/ {

	typedef sequence_parser_type::container parser_container;
	// parses sequence
	if (parser.get() == NULL) {
		return;
	}
	parser->parse(this->getSequence());

	// gets elements from parser
	parser_container parsed_elements = parser->getElements();
	
	// creates container - map with elements as keys and their abundances
	// as values.
	for (parser_container::const_iterator it = parsed_elements.begin(); 
							   it != parsed_elements.end(); ++it) {
		container::key_type element = alphabet.getElement(it->first);
		elements[element] = it->second;
	}
}


void ComposedElement::initializeElements(const std::vector<unsigned int>& decomposition, 
		const Alphabet& alphabet) {
	
	for (std::vector<unsigned int>::size_type i = 0; 
			i < decomposition.size() && i < alphabet.size(); ++i) {
		if (decomposition[i] != 0) {
			container::key_type element = alphabet.getElement(i);
			elements[element] = decomposition[i];
		}
	}
}


void ComposedElement::updateSequence(const std::vector<name_type>* elements_order) {
						
	// initializes stream to store 
	std::ostringstream sequence_stream;
	
	// if a elements (in fact element's names) order is given
	if (elements_order != 0) {
		typedef std::vector<name_type> order_container_type;
		typedef std::map<order_container_type::size_type, 
					std::pair<name_type, container::value_type> > 
					elements_order_container_type;
		
		// collects element's name and abundance 
		// in order they appear in a given sequence
		elements_order_container_type elements_in_order;
		for (order_container_type::const_iterator it = elements_order->begin();
									   it != elements_order->end(); ++it) {
			// checks if there is an element with
			// name from the given order
			container::const_iterator element_it = std::find_if(
			 	elements.begin(), elements.end(), FindElementByName(*it));
			if (element_it != elements.end()) {
				// adds element's name to the sequence
				sequence_stream << element_it->first.getName();
				// adds element's abundance to the sequence, if abundance > 1
				if (element_it->second > 1) {
					sequence_stream << element_it->second;
				}
			} 
		}
	} else {
		// fills stream in a random order
		for (container::const_iterator it = elements.begin(); 
									   it != elements.end(); ++it) {
			name_type element_name = it->first.getName();
			sequence_stream << element_name;
			if (it->second > 1) {
				sequence_stream << it->second;
			}
		}
	}
	
	this->setSequence(sequence_stream.str());	
}


void ComposedElement::updateIsotopeDistribution(){
	// initializes store where folded distributions will be collected
	isotopes_type isodistr;
	// loops through elements and folds them first with themselves so often
	// as their abundance in molecule is and then folds the result into store.
	for (container::const_iterator it = elements.begin(); 
								   it != elements.end(); ++it) {
		isotopes_type element_isodistr = it->first.getIsotopeDistribution();
		element_isodistr *= it->second;
		isodistr *= element_isodistr;
	}
	this->setIsotopeDistribution(isodistr);
}


std::ostream& operator << (std::ostream& os, 
							const ComposedElement& composed_element) {
	for (ComposedElement::container::const_iterator 
				it = composed_element.getElements().begin(); 
				it != composed_element.getElements().end(); ++it) {
		os << it->first << ' ' <<  it->second << '\n';	
	}
	return os;
}


} // namespace ims
