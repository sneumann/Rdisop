
#include <cassert>
#include <ims/characteralphabet.h>

using namespace std;
using namespace ims;

CharacterAlphabet::CharacterAlphabet(const Alphabet& alphabet) : masses(256, -1.0) {
	// TODO: move to configure
	assert(sizeof(char)==1);
	for (size_t i=0; i<alphabet.size(); ++i) {
		string s = alphabet.getName(i);
		if (s.length()!=1) throw Exception("Invalid element name: \"" + s + "\" (expected only one character)");
		// TODO: can this assertion ever fail?
		assert(s[0]>=0);
		size_t index = (size_t)(s[0]);
		masses[index]=alphabet.getElement(i).getMass();
	}
}

Alphabet::mass_type CharacterAlphabet::getMass(const char& name) const 
    /*throw (UnknownCharacterException)*/ {
	if (name<0) throw UnknownCharacterException();
	Alphabet::mass_type result = masses[(size_t)name];
	if (result<0.0) throw UnknownCharacterException();
	return result;
}
