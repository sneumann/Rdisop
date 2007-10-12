#ifndef IMS_ALPHABETGETMASSFUNCTOR_H
#define IMS_ALPHABETGETMASSFUNCTOR_H

#include <ims/alphabet.h>

namespace ims {


class AlphabetGetMassFunctor {
	public:
		typedef Alphabet alphabet_type;
		double operator() (const alphabet_type& alphabet, const std::string& str) {
			return alphabet.getMass(str); }
};

} // namespace ims

#endif
