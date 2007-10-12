#include <sstream>

#include <ims/base/parser/distributedalphabettextparser.h>

/**
 * Parses the data from the stream \c is \c. 
 * While loading the following is ignored:
 * - white space
 * - lines containing only white space
 * - lines starting with '#' (even after leading whitespace, but not after anything else) 
 * 
 * @param is The input stream to be parsed.
 */
void ims::DistributedAlphabetTextParser::parse(std::istream& is) {
	// first clears all data
	this->clear();
	
	// initializes local variables
	std::string line;
	std::string name;
	double mass;
	double probability;
	// reads and works on the stream line by line
	while (std::getline(is, line)) {
		size_t i = line.find_first_not_of(" \t");
		if (i == std::string::npos || line[i] == '#') {
			continue; // skips comment lines
		}
		std::istringstream input(line);
		input >> name >> mass >> probability;
		elements.insert(std::make_pair(name, mass));
		elementsDistribution.insert(std::make_pair(name, probability));
	}
}

/**
 * Clears all persistent data.
 */	
void ims::DistributedAlphabetTextParser::clear() {
	elements.clear();
	elementsDistribution.clear();
}
