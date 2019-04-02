#ifndef IMS_STANDARDMOLECULESEQUENCEPARSER_H
#define IMS_STANDARDMOLECULESEQUENCEPARSER_H

#include <map>
#include <string>
#include <ims/base/parser/abstractmoleculesequenceparser.h>

namespace ims {

/**
 * @brief Parses molecule sequences that are defined in tex-notation, i.e. CH_{4}.
 */
class StandardMoleculeSequenceParser : public AbstractMoleculeSequenceParser {
	
	public:
	
		StandardMoleculeSequenceParser() : AbstractMoleculeSequenceParser() {}
		
		StandardMoleculeSequenceParser(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
			
		virtual void parse(const sequence_type& sequence) 
									/*throw (UnknownCharacterException)*/;
									
		virtual ~StandardMoleculeSequenceParser() {}
	
	private:
		
		container parseElements(const sequence_type& sequence)
									/*throw (UnknownCharacterException)*/;
		
		void addElement(const sequence_type& name, number_type amount, 
											container& elements);
};

} // namespace ims

#endif // IMS_STANDARDMOLECULESEQUENCEPARSER_H
