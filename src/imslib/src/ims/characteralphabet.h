#ifndef IMS_CHARACTER_ALPHABET_H
#define IMS_CHARACTER_ALPHABET_H

#include <vector>

#include "alphabet.h"
#include "base/exception/exception.h"
#include "base/exception/unknowncharacterexception.h"

/** Implementation of a weighted alphabet containing only single characters.
  * It is kept very simple and designed for fast access to weights.
  * @note May be removed when Alphabet or AlphabetWeights or whatever
  * is capable of providing fast access to masses by element name.
  */
class CharacterAlphabet {
	public:
		CharacterAlphabet(const ims::Alphabet& alphabet);
		ims::Alphabet::mass_type getMass(const char& name) const 
		    /*throw (ims::UnknownCharacterException)*/;
	private:
		std::vector<ims::Alphabet::mass_type> masses;
};

#endif
