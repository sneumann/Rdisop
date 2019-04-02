#ifndef IMS_ABSTRACTMOLECULESEQUENCEPARSER_H
#define IMS_ABSTRACTMOLECULESEQUENCEPARSER_H

#include <map>
#include <string>
#include <ims/base/exception/unknowncharacterexception.h>

namespace ims {

class AbstractMoleculeSequenceParser {

	public:

		typedef std::string sequence_type;	

		typedef unsigned int number_type;		

		typedef std::map<sequence_type, number_type> container;
		
		AbstractMoleculeSequenceParser(): multiplicator(1) {}
								
		virtual void parse(const sequence_type& sequence) 
						/*throw (UnknownCharacterException)*/ = 0;

		const container& getElements() const { return elements; }
		
		void setElements(const container& elements) 
									{ this->elements = elements; }
		
		number_type getMultiplicator() const { return multiplicator; }
		
		void setMultiplicator(number_type multiplicator)
									{ this->multiplicator = multiplicator; }
									
		virtual ~AbstractMoleculeSequenceParser() {}

	private:
		container elements;
		number_type multiplicator;
};

} // namespace ims

#endif // IMS_MOLECULESEQUENCEPARSER_H
