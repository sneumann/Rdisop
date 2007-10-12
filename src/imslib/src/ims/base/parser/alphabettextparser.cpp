#include <sstream>

#include <ims/base/parser/alphabettextparser.h>

/**
 * Parses the data from the stream \c is \c. 
 * While loading the following is ignored:
 * - white space
 * - lines containing only white space
 * - lines starting with '#' (even after leading whitespace, but not after anything else) 
 * 
 * @param is The input stream to be parsed.
 */
void ims::AlphabetTextParser::parse(std::istream& is) {
	// first make sure the store is empty
	elements.clear();
	std::string line;
	std::string name;
	const std::string delimits(" \t"), comments("#");
	double mass;
	while (std::getline(is, line)) {
		std::string::size_type i = line.find_first_not_of(delimits);
		if (i == std::string::npos || comments.find(line[i]) != std::string::npos) {
			continue; // skip comment lines
		}
		std::istringstream input(line);
		input >> name >> mass;
		elements.insert(std::make_pair(name, mass));
	}
}

