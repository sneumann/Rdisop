#ifndef IMS_MASSESPARSER_H
#define IMS_MASSESPARSER_H

#include <vector>
#include <string>
#include <istream>
#include <fstream>
#include <ims/base/exception/ioexception.h>

namespace ims {

template <typename MassesType = double,
		  typename InputSource = std::istream>
class MassesParser {
	public:
		typedef std::vector<MassesType> ContainerType;
		/**
		 * Loads the data from the InputSource with the name \c fname \c. 
		 * If there is an error occured while reading data from InputSource, 
		 * \c IOException \c is thrown.
		 * 
		 * @param fname The name of the input source.
		 */
		void load(const std::string& fname) /*throw (IOException)*/;
		
		/**
		 * Gets loaded data.
		 * 
		 * @return The loaded data.
		 */
		virtual const ContainerType& getElements() = 0;

		/**
		 * Parses the the given input source \c is \c.
		 */	
		virtual void parse(InputSource& is) = 0;
		
		/**
		 * Destructor.
		 */
		virtual ~MassesParser() {}		
};

template <typename MassesType, typename InputSource>
void MassesParser<MassesType, InputSource>::load(const std::string& fname) /*throw (IOException)*/ {
	std::ifstream ifs(fname.c_str());
	if (!ifs) {
		throw IOException("unable to open resource file with masses: " + fname + "!");
	}
	this->parse(ifs);	
}

}

#endif // IMS_MASSESPARSER_H
