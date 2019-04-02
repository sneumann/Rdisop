/**
 * moleculeionchargemodificationparser.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 *
 */

#include <sstream>
#include <ims/base/parser/moleculeionchargemodificationparser.h>

namespace ims {

MoleculeIonChargeModificationParser::MoleculeIonChargeModificationParser(
					const ion_name_type& sequence) /*throw (UnknownCharacterException)*/ {
	parse(sequence);
}

MoleculeIonChargeModificationParser& MoleculeIonChargeModificationParser::
operator =(const MoleculeIonChargeModificationParser& parser) {
	if (this != &parser) {
		mass_factor = parser.mass_factor;
		charge = parser.charge;
		ions = parser.ions;
	}
	return *this;
}


void MoleculeIonChargeModificationParser::parse(const ion_name_type& sequence) 
/*throw (UnknownCharacterException)*/ {
	
	this->clear();
	
	if (sequence.empty()) {
		throw UnknownCharacterException("Parsed ion modification is empty!");
	}
		
	if (sequence[0] != MOLECULE_SYMBOL) {
		throw UnknownCharacterException("Ion modification \"" + sequence + 
										"\" has to start with letter '" + "'" + MOLECULE_SYMBOL + "!");
	}

	size_type letter_pos = 1, len = sequence.length();
	
	//////////////////////////////////////////////////////////
	//////////// parsing molecule's mass factor //////////////
	//////////////////////////////////////////////////////////
	if (std::isdigit(sequence[letter_pos])) {
		size_type number_range = 1;
		for (;  letter_pos + number_range < len && 
	        	std::isdigit(sequence[letter_pos + number_range]); ++number_range) {
		}
		std::istringstream is(sequence.substr(letter_pos, number_range));
		if (!(is >> mass_factor)) {
			throw UnknownCharacterException("Ion modification \"" + sequence + "\" cannot be parsed!" + 
											" Stopped at digit '" + sequence[letter_pos] + "'!");
		}
		letter_pos += number_range;
	}

	//////////////////////////////////////////////////////////
	//////////////// parsing molecule's charge ///////////////
	//////////////////////////////////////////////////////////
	if (std::isdigit(sequence[len-1])) {
		size_type number_range = 1;
		for (;  len - number_range > letter_pos && 
	        std::isdigit(sequence[len - number_range - 1]); ++number_range) {
		}
		std::istringstream is(sequence.substr(len - number_range));
		if (!(is >> charge)) {
			throw UnknownCharacterException("Ion modification \"" + sequence + "\" cannot be parsed!" + 
											" Stopped at digit '" + sequence[len-number_range] + "'!");
		}
		len -= number_range;
	}

				
	//////////////////////////////////////////////////////////
	//////////////// parsing ionized element /////////////////
	//////////////////////////////////////////////////////////
			
	if (letter_pos >= len) {
		throw UnknownCharacterException("Ion modification doesn't contain ionized element!");
	}
	
	// has to be created as temporaty variable since ion object to whom it will be assigned is not yet initialized
	bool is_ion_added = true;
	if (sequence[letter_pos] == ANION_CHARGE_SYMBOL) {
		is_ion_added = false;
		++letter_pos;
	}

	// main loop: on each iteration ion object is created and filled out with its properties: elements, is_ion_cation and is_ion_added
	do {

		ion_type ion;

		// assigns the property which is already known
		ion.is_ion_added = is_ion_added;

		// collects ion elements. NOTE: ion could be not only atoms H, Cl etc but a composed element such as NH4 
		for (size_type i = 0;	letter_pos < len && 
							std::isalpha(sequence[letter_pos]) && 
							std::isupper(sequence[letter_pos]); ++i) {

			///////////////////////////////////////////////////////////////////////////////////////////	
			// gets new letter (element name)
			size_type number_pos_beg = 1;
			for (;std::isalpha(sequence[letter_pos + number_pos_beg]) && 
			      std::islower(sequence[letter_pos + number_pos_beg]) && 
		    	  letter_pos + number_pos_beg < len; ++number_pos_beg) {
			}
			ion_name_type letter = sequence.substr(letter_pos, number_pos_beg);
// 		std::cout << "new letter: " << letter << "number_pos_beg: " << number_pos_beg << std::endl;
			///////////////////////////////////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////////////////////////////////	
			// gets amount of elements (with the name 'letter')
			size_type number_range = 0;
			for (;  letter_pos + number_pos_beg + number_range < len && 
			        std::isdigit(sequence[letter_pos + number_pos_beg + number_range]); ++number_range) {
			}
			elements_amount_type amount = 1;
			if (number_range > 0) {
				std::istringstream is(sequence.substr(letter_pos+number_pos_beg, number_range));
				if (!(is >> amount)) {
					throw UnknownCharacterException("Error: sequence \"" + sequence + "\" cannot be parsed!" + 
							" Stopped at digit '" + sequence[letter_pos+number_pos_beg] + "'!");
				}
			}
// 		std::cout << "new amount: " << amount << "number_pos_end:" << number_pos_end << std::endl; 
			///////////////////////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////////////////////	
			// checks if elements already have letter
			ion_elements_type::iterator it = ion.elements.find(letter);
			// if true, accumulates the amount of letter, else - adds new element to elements
			if (it != ion.elements.end()) {
				it->second += amount;
			} else {
				ion.elements[letter] = amount;
			}
			letter_pos += number_pos_beg + number_range;
			///////////////////////////////////////////////////////////////////////////////////////////	
			
		} // for loop	

		if (letter_pos < len) {
			ion.is_ion_cation = true;
			if (sequence[letter_pos] == ANION_CHARGE_SYMBOL || 
				sequence[letter_pos] == CATION_CHARGE_SYMBOL) {
				
				// checks if the current letter position is the last one
				// if yes
				if (letter_pos+1 < len) {
					// checks if two charge symbols are placed after each other
					if (sequence[letter_pos+1] == ANION_CHARGE_SYMBOL ||
						sequence[letter_pos+1] == CATION_CHARGE_SYMBOL) {
						
						// sets the ion's sign (cation/anion) with respect to the first of two charge symbols
						ion.is_ion_cation = !(sequence[letter_pos] == ANION_CHARGE_SYMBOL);
		
						// moves a pointer on a second charge symbol
						++letter_pos;
					}
					// keeps value for the next ion
					is_ion_added = (sequence[letter_pos] == CATION_CHARGE_SYMBOL);

				// if no, the sign under current letter defines the property is_ion_cation for the current ion 
				} else {
					ion.is_ion_cation = !(sequence[letter_pos] == ANION_CHARGE_SYMBOL);
				}
			} else {
				throw UnknownCharacterException("Error: sequence '" + sequence + 
				"' has a wrong charge symbol (character '" + sequence[letter_pos] + "')!");
			}
			
			// adds the ion to the container (ion properties were filled out in this iteration 
			// of the do-while loop)
			ions.push_back(ion);
		}
		
	} while ( ++letter_pos < len && 
			  ( sequence[letter_pos-1] == ANION_CHARGE_SYMBOL ||
				sequence[letter_pos-1] == CATION_CHARGE_SYMBOL) );
		
	if (letter_pos < len) {
		throw UnknownCharacterException("Error: sequence \"" + sequence + 
		"\" can be parsed only until the character '" + sequence[letter_pos-1] + "'!");
	}

}

void MoleculeIonChargeModificationParser::clear() {
	mass_factor = 1;
	charge = 1;
	ions.clear();	
}
		
} // namespace ims
