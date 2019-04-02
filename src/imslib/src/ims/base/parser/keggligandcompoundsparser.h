#ifndef IMS_KEGGLIGANDCOMPOUNDSPARSER_H
#define IMS_KEGGLIGANDCOMPOUNDSPARSER_H

#include <map>
#include <string>
#include <ims/base/parser/abstractmoleculesequenceparser.h>

namespace ims {

class KeggLigandCompoundsParser : public AbstractMoleculeSequenceParser {

	public:
	
		KeggLigandCompoundsParser() : AbstractMoleculeSequenceParser() {}
		
		KeggLigandCompoundsParser(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
			
		virtual void parse(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
		
		virtual ~KeggLigandCompoundsParser() {}
};

} // namespace ims

#endif // IMS_KEGGLIGANDCOMPOUNDSPARSER_H
