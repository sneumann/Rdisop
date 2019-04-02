#ifndef IMS_ALPHABETPARSER_H
#define IMS_ALPHABETPARSER_H

#include <fstream>
#include <istream>
#include <map>
#include <string>
#include <ims/base/exception/ioexception.h>

namespace ims {

/**
 * An abstract templatized parser to load the data that is used to initialize @c Alphabet objects. 
 * @c AlphabetParser reads the input source, which is given as a template parameter @c InputSource , by
 * @c load (const std::string& fname) function where @c fname is the source name. 
 * Loaded data can be retrieved by calling @c getElements().
 * 
 * @see Alphabet
 */
template <typename AlphabetElementType = double, 
		  typename Container = std::map<std::string, AlphabetElementType>, 
		  typename InputSource = std::istream>
class AlphabetParser {
	public:
		/**
		 * Type of data to be loaded.
		 */
		typedef Container ContainerType;
		
		/**
		 * Loads the data from the InputSource with the name @c fname. 
		 * If there is an error occured while reading data from InputSource, 
		 * @c IOException is thrown.
		 * 
		 * @param fname The name of the input source.
		 */
		void load(const std::string& fname) /*throw (IOException)*/;
		
		/**
		 * Gets the data that was loaded. 
		 * 
		 * @return The data.
		 */
		virtual ContainerType& getElements() = 0;
		
		/**
		 * Parses the the given input source \c is \c.
		 */	
		virtual void parse(InputSource& is) = 0;
		
		/**
		 * Destructor.
		 */
		virtual ~AlphabetParser() {}
};

template <typename AlphabetElementType, typename Container, typename InputSource>
void AlphabetParser<AlphabetElementType, Container, InputSource>::load(const std::string& fname) /*throw (IOException)*/ {
	std::ifstream ifs(fname.c_str());
	if (!ifs) {
		throw IOException("unable to open alphabet resource file: " + fname + "!");
	}
	this->parse(ifs);
}

}

#endif // IMS_ALPHABETPARSER_H
