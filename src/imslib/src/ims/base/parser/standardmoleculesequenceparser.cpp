/**
 * standardmoleculesequenceparser.cpp
 *
 * Author: Florian Rasche <m3rafl@minet.uni-jena.de>
 *
 */

#include <sstream>
#include <iostream>
#include <ims/base/parser/standardmoleculesequenceparser.h>

namespace ims {

StandardMoleculeSequenceParser::StandardMoleculeSequenceParser(const sequence_type& 
								sequence) /*throw (UnknownCharacterException)*/ {
	this->parse(sequence);
}

void StandardMoleculeSequenceParser::parse(const sequence_type& sequence) 
											/*throw (UnknownCharacterException)*/ {
	
	const sequence_type delimiters(" \t");
	
	// skips delimiters
	sequence_type::size_type start_pos = 
								sequence.find_first_not_of(delimiters), 
			       			 end_pos = 
			       			 	sequence.find_last_not_of(delimiters) + 1;

	if (start_pos == sequence_type::npos) {
		throw UnknownCharacterException("Empty sequence cannot be parsed!");		
	}
	
	//////////////////////////////////////////////////////////////////////
	//// checks for multiplicator in the beginning of molecule formula ///
	//////////////////////////////////////////////////////////////////////
	sequence_type::size_type multiplicator_range = 0;
	for (;  start_pos < end_pos && 
		std::isdigit(sequence[start_pos + multiplicator_range]); ++multiplicator_range) {
	}
	number_type multiplicator = 1;
	if (multiplicator_range > 0) {
		std::istringstream is(sequence.substr(start_pos, multiplicator_range));
		if ((is >> multiplicator)) {
		}
		start_pos += multiplicator_range;
	}
	//////////////////////////////////////////////////////////////////////
	
	container elements = parseElements(sequence.substr(start_pos, end_pos - start_pos));

	this->setMultiplicator(multiplicator);	
	this->setElements(elements);
}

StandardMoleculeSequenceParser::container 
StandardMoleculeSequenceParser::parseElements(const sequence_type& sequence)
		 /*throw (UnknownCharacterException)*/ {
	
	container elements;
	
	const sequence_type openingBrackets("("), closingBrackets(")"), openSpecialBrackets("{"), closeSpecialBrackets("}"), underLine("_");
	
	sequence_type::size_type start_pos = 0, 
			       			 end_pos = sequence.size();
//	std::cout << "parsing sequence " << sequence << " from index " << start_pos << " until index " << end_pos-1 << '\n';
	for (; start_pos < end_pos ;) {

		if (openingBrackets.find(sequence[start_pos]) != sequence_type::npos) {
			
			sequence_type::size_type subsequence_end_pos = sequence.find_first_of(closingBrackets, start_pos);
			
			// checks if there is a closing bracket as a pair to the opening one
			if (subsequence_end_pos == sequence_type::npos) {
				throw UnknownCharacterException("Sequence " + sequence + " has non-closed bracket!");	
			}
			// checks if there are enclosing brackets.
			sequence_type::size_type another_subsequence_start_pos = sequence.find_first_of(openingBrackets, start_pos+1);
			if (another_subsequence_start_pos != sequence_type::npos &&
			another_subsequence_start_pos < subsequence_end_pos) {
				throw UnknownCharacterException("Sequence " + sequence + " consists of enclosed brackets! Parsing of enclosed brackets is not supported.");	
			}
//			std::cout << "subsequence starts at " << (start_pos + 1) <<  " with letter "
//					  << sequence[start_pos+1] << " ends at " << (subsequence_end_pos - 1) 
//					  << " with letter " << sequence[subsequence_end_pos - 1] << '\n';
			
			container subsequence_elements = parseElements(sequence.substr(start_pos + 1, subsequence_end_pos - start_pos - 1));
			sequence_type::size_type number_range = 0, special_char_offset = 1; // Offset because of cloing sequence ")_{"
			if (sequence.substr(subsequence_end_pos + special_char_offset, 1) == underLine) special_char_offset += 2; //Assume that } follows _
			for (;  subsequence_end_pos + special_char_offset + number_range < end_pos && 
			        std::isdigit(sequence[subsequence_end_pos + special_char_offset + number_range]); ++number_range) {
			}
			number_type amount = 1;
			if (number_range > 0) {
				std::istringstream is(sequence.substr(subsequence_end_pos + 1, number_range));
				if ((is >> amount)) {
//					std::cout << "subsequence is a chain with " << amount << " repetitions\n";
				}
			}
			for (container::const_iterator it = subsequence_elements.begin(); 
							it != subsequence_elements.end(); ++it) {
				addElement(it->first, it->second*amount, elements);
			}
			start_pos += subsequence_end_pos - start_pos + 1 + number_range;
			if(sequence.substr(start_pos, 1) == closeSpecialBrackets) ++start_pos; // If there are closing brackets continue one position further
//			std::cout << "subsequence is parsed, continuing from letter " << sequence[start_pos] << '\n';

		}	else if ((std::isalpha(sequence[start_pos]) 
				  && std::isupper(sequence[start_pos]))
				 || sequence[start_pos]=='+' 
				 || sequence[start_pos]=='-' 
				 ) {
		
			sequence_type::size_type number_pos_beg = 1;

			for (;std::isalpha(sequence[start_pos + number_pos_beg]) && 
			      std::islower(sequence[start_pos + number_pos_beg]) && 
		    		  start_pos + number_pos_beg < end_pos; ++number_pos_beg) {
			}

			sequence_type letter = sequence.substr(start_pos, number_pos_beg);
			if (sequence.substr(start_pos+number_pos_beg, 1) == underLine) number_pos_beg += 2; // Assume that underline is followed by {

// 			std::cout << "new letter: " << letter << "number_pos_beg: " << number_pos_beg << std::endl;
			sequence_type::size_type number_range = 0;
			for (;  start_pos + number_pos_beg + number_range < end_pos && 
		    	    std::isdigit(sequence[start_pos + number_pos_beg + number_range]); ++number_range) {
			}
			
			number_type amount = 1;
			if (number_range > 0) {
				std::istringstream is(sequence.substr(start_pos+number_pos_beg, number_range));
				if (!(is >> amount)) {
					throw UnknownCharacterException("Sequence \"" + sequence + "\" cannot be parsed!" + " Stopped at digit '" + sequence[start_pos+number_pos_beg] + "'!");
				}
			}
// 			std::cout << "new amount: " << amount << "number_pos_end:" << number_pos_end << std::endl; 
			addElement(letter, amount, elements);
			start_pos += number_pos_beg + number_range;
			if(sequence.substr(start_pos, 1) == closeSpecialBrackets) ++start_pos; // If there are closing brackets continue one position further
		} else {
			throw UnknownCharacterException("Sequence \"" + sequence+"\" can be parsed until the character '" 
			+ sequence[start_pos-1] + "'. The rest subsequence " + sequence.substr(start_pos)+ " cannot be parsed!");	
		}
	}
			
	return elements;
}

void StandardMoleculeSequenceParser::addElement(const sequence_type& name, 
			number_type amount, 	container& elements) {
	// checks if elements already have letter
	container::iterator it = elements.find(name);
	// if true, accumulates the amount of letter
	if (it != elements.end()) {
		it->second += amount;
	// else, adds a new element to elements
	} else {
		elements[name] = amount;
	}
}

} // namespace ims
