#ifndef IMS_DISTRIBUTEDALPHABETTEXTPARSER_H
#define IMS_DISTRIBUTEDALPHABETTEXTPARSER_H

#include <map>
#include <string>
#include <istream>
#include <ims/base/parser/distributedalphabetparser.h>

namespace ims {

/**
 * Implements abstract \c DistributedAlphabetParser \c to read data from the plain text format.
 * \c DistributedAlphabetTextParser \c parses the data source using overriden \c parse(std::istream&) \c function
 * and stores the parsed data permanently. To retrieve the data, functions \c getElements() \c and
 * \c getElementsDistribution() can be used. 
 * 
 * @DistributedAlphabetTextParser
 */
class DistributedAlphabetTextParser : public DistributedAlphabetParser<> {
	private:
		/**
		 * @see DistributedAlphabetParser<>::DistributionContainerType
		 */
		typedef DistributedAlphabetParser<>::DistributionContainerType DistributionContainerType;
		
		/**
	 	 * The parsed data.
	 	 */ 
	 	ContainerType elements;
		
		/**
		 * The parsed data distribution.
		 */
		DistributionContainerType elementsDistribution;

		/**
		 * Clears all persistent data.
		 */		
		void clear();
	public:
		/**
		 * Gets the parsed data.
		 * 
		 * @return The parsed data.
		 * 
		 * @see AlphabetTextParser::getElements()
		 */
		virtual ContainerType& getElements() { return elements; };
		
		/**
		 * Gets the data distribution.
		 * 
		 * @return The data distribution.
		 */
		virtual DistributionContainerType& getElementsDistribution() { return elementsDistribution; };
		
		/**
		 * Parses the input stream \c is \c.
		 * 
		 * @param is The input stream to be parsed
		 * 
		 * @see AlphabetTextParser::parse(std::istream&)
		 */
		 virtual void parse(std::istream& is);
};

}

#endif // IMS_DISTRIBUTEDALPHABETTEXTPARSER_H
