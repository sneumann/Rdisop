#ifndef IMS_ALPHABETTEXTPARSER_H
#define IMS_ALPHABETTEXTPARSER_H

#include <ims/base/parser/alphabetparser.h>

namespace ims {

/**
 * Implements abstract \c AlphabetParser \c to read data from the plain text format.
 * \c AlphabetTextParser \c parses the data source using overriden \c parse(std::istream&) \c function
 * and stores the parsed data permanently. That can be retrieved by \c getElements() \c function. 
 * 
 * @DistributedAlphabetTextParser
 */
class AlphabetTextParser : public AlphabetParser<> {
	private:
		/**
	 	 * The parsed data.
	 	 */ 
		ContainerType elements;
	public:
		/**
		 * Gets the parsed data.
		 * 
		 * @return The parsed data.
		 */
		virtual ContainerType& getElements() { return elements; };
		
		/**
		 * Parses the input stream \c is \c.
		 * 
		 * @param is The input stream to be parsed
		 */
		virtual void parse(std::istream& is);
};

}

#endif // IMS_ALPHABETTEXTPARSER_H
