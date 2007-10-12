#ifndef IMS_MASSESTEXTPARSER_H
#define IMS_MASSESTEXTPARSER_H

#include <ims/base/parser/massesparser.h>

namespace ims {

class MassesTextParser : public MassesParser<> {
	public:
		virtual const MassesParser<>::ContainerType& getElements() {
			return elements; };
		/**
		 * Parses the input stream \c is \c.
		 * 
		 * @param is The input stream to be parsed
		 */
		virtual void parse(std::istream& is);			
	private:
		MassesParser<>::ContainerType elements;	
		void addElement(const std::string& str);
};

}

#endif // IMS_MASSESTEXTPARSER_H
