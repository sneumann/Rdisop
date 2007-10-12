#include <sstream>
#include <ims/base/parser/massestextparser.h>
#include <ims/base/exception/ioexception.h>
#include <iostream>

/**
 * Parses the data from the stream \c is. The format is one mass per line.
 * Leading whitespace (tab and space), empty lines, and lines starting
 * with one of #;!/ (also after leading whitespace) are ignored.
 *
 * @param is The input stream to be parsed.
 */
void ims::MassesTextParser::parse(std::istream& is) {
	// clears the store
	elements.clear();
	const std::string delims(" \t"), comments("#;!/");
	std::string line;
	while (std::getline(is, line)) {
		//	sets the starting index
		std::string::size_type begIndex = line.find_first_not_of(delims);

		// ignore comments and empty lines
		if (begIndex == std::string::npos ||
			comments.find(line[begIndex]) != std::string::npos) {
			continue;
		}

		std::string::size_type endIndex = line.find_first_of(delims, begIndex);
		
		std::string token;
		if (endIndex == std::string::npos) {
			token = line.substr(begIndex);
		}
		else {
			token = line.substr(begIndex, endIndex - begIndex);
		}

		double dblElement;

		// try to parse token. Ignore line if not successful
		if (std::istringstream(token) >> dblElement) {
			elements.push_back(dblElement);
		}
	}
}
