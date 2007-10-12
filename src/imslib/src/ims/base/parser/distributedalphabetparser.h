#ifndef IMS_DISTRIBUTEDALPHABETPARSER_H
#define IMS_DISTRIBUTEDALPHABETPARSER_H

#include <map>
#include <string>
#include <ims/base/parser/alphabetparser.h>

namespace ims {

/**
 * An abstract templatized parser to load the data that is used to initialize 
 * \c DistributedAlphabet \c objects.
 * \c DistributedAlphabetParser \c uses the \c AlphabetParser \c functionaly and
 * defines some other methods specific to the data for 
 * \c DistributedAlphabet \c initialization.
 */
template <typename DistributionContainer = std::map<std::string, double> >
class DistributedAlphabetParser : public AlphabetParser<> {
	public:
		/**
		 * Type of the data distribution container.
		 */
		typedef DistributionContainer DistributionContainerType;
		
		/**
		 * Gets the data distribution.
		 */
		virtual DistributionContainerType& getElementsDistribution() = 0;
};

}

#endif // IMS_DISTRIBUTEDALPHABETPARSER_H
