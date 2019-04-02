/**
 * keggligandcompoundsparser.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 *
 */

#include <sstream>
#include <iostream>
#include <ims/base/parser/keggligandcompoundsparser.h>

namespace ims {

KeggLigandCompoundsParser::KeggLigandCompoundsParser(const sequence_type& 
								sequence) /*throw (UnknownCharacterException)*/ {
	this->parse(sequence);
}

void KeggLigandCompoundsParser::parse(const sequence_type& sequence) 
											/*throw (UnknownCharacterException)*/ {
	
	const sequence_type delimiters(" \t"), rest_header(".R*+(");
	
	// skips delimiters
	sequence_type::size_type letter_pos = 
								sequence.find_first_not_of(delimiters), 
			       			 last_pos = 
			       			 	sequence.find_last_not_of(delimiters) + 1;

	if (letter_pos == sequence_type::npos) {
		throw UnknownCharacterException("Empty sequence cannot be parsed!");		
	}
	
	//////////////////////////////////////////////////////////////////////
	//// checks for multiplicator in the beginning of molecule formula ///
	//////////////////////////////////////////////////////////////////////
	sequence_type::size_type multiplicator_range = 0;
	for (;  letter_pos < last_pos && 
		std::isdigit(sequence[letter_pos + multiplicator_range]); ++multiplicator_range) {
	}
	number_type multiplicator = 1;
	if (multiplicator_range > 0) {
		std::istringstream is(sequence.substr(letter_pos, multiplicator_range));
		if ((is >> multiplicator)) {
		}
		letter_pos += multiplicator_range;
	}
	this->setMultiplicator(multiplicator);
	//////////////////////////////////////////////////////////////////////
	
	container elements;
	
	for (;	letter_pos < last_pos && 
			( std::isalpha(sequence[letter_pos]) && std::isupper(sequence[letter_pos])) ||
			( rest_header.find(sequence[letter_pos]) != std::string::npos);) {
		
		///////////////////////////////////////////////////////////////
		//// checks if you reached the end of sequence
		///////////////////////////////////////////////////////////////		
		if (rest_header.find(sequence[letter_pos]) != std::string::npos) {
//			std::cout << "molecule's tale: " << sequence.substr(letter_pos) << '\n';			
			letter_pos = last_pos;
			break;
		}
		///////////////////////////////////////////////////////////////
// 		std::cout << "letter_pos:" << letter_pos << std::endl;

		sequence_type::size_type number_pos_beg = 1;

		for (;std::isalpha(sequence[letter_pos + number_pos_beg]) && 
		      std::islower(sequence[letter_pos + number_pos_beg]) && 
		      letter_pos + number_pos_beg < last_pos; ++number_pos_beg) {
		}

		sequence_type letter = sequence.substr(letter_pos, number_pos_beg);
// 		std::cout << "new letter: " << letter << "number_pos_beg: " << number_pos_beg << std::endl;
		sequence_type::size_type number_range = 0;
		for (;  letter_pos + number_pos_beg + number_range < last_pos && 
		        std::isdigit(sequence[letter_pos + number_pos_beg + number_range]); ++number_range) {
		}
		number_type amount = 1;
		if (number_range > 0) {
			std::istringstream is(sequence.substr(letter_pos+number_pos_beg, number_range));
			if (!(is >> amount)) {
				throw UnknownCharacterException("Sequence \"" + sequence + "\" cannot be parsed!" + " Stopped at digit '" + sequence[letter_pos+number_pos_beg] + "'!");
			}
		}
// 		std::cout << "new amount: " << amount << "number_pos_end:" << number_pos_end << std::endl; 
		// checks if elements already have letter
		container::iterator it = elements.find(letter);
		// if true, accumulates the amount of letter, else - adds new element to elements
		if (it != elements.end()) {
			it->second += amount;
		} else {
			elements[letter] = amount;
		}
		letter_pos += number_pos_beg + number_range;
	}
		
	if (letter_pos < last_pos) {
		throw UnknownCharacterException("Sequence \"" + sequence+"\" can be parsed until the character '" 
		+ sequence[letter_pos-1] + "'. The rest subsequence " + sequence.substr(letter_pos)+ " cannot be parsed!");
	}
	this->setElements(elements);
}

} // namespace ims
