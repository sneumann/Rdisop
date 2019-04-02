#ifndef IMS_MOLECULESEQUENCEPARSER_H
#define IMS_MOLECULESEQUENCEPARSER_H

#include <map>
#include <string>
#include <ims/base/parser/abstractmoleculesequenceparser.h>

namespace ims {

class MoleculeSequenceParser : public AbstractMoleculeSequenceParser {
	
	public:
	
		MoleculeSequenceParser() : AbstractMoleculeSequenceParser() {}
		
		MoleculeSequenceParser(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
			
		virtual void parse(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
									
		virtual ~MoleculeSequenceParser() {}
	
	private:
		
		container parseElements(const sequence_type& sequence)
									/*throw (UnknownCharacterException)*/;
		
		void addElement(const sequence_type& name, number_type amount, 
											container& elements);
};

} // namespace ims

#endif // IMS_MOLECULESEQUENCEPARSER_H
