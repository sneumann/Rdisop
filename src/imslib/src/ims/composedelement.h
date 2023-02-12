#ifndef IMS_COMPOSEDELEMENT_H
#define IMS_COMPOSEDELEMENT_H

#include <map>
#include <ostream>
#include <memory>
#include <ims/element.h>
#include <ims/alphabet.h>
#include <ims/elementsortcriteria.h>
#include <ims/base/parser/abstractmoleculesequenceparser.h>

namespace ims {

/**
 * @brief Represents a bio-chemical molecule with a name, a sequence of simple elements (atoms) or other molecules it consists of) and an isotope distribution.
 * 
 * Simulates non-trivial bio-chemical molecule that has a name (a short symbol
 * to be used in sequences of other molecules), a sequence (atoms or other
 * elements it consists of) and an isotopic distribution. 
 * To show an interrelation between molecule's name and sequence, here is an
 * example of those for amino acid Tyrosine:
 * 		name - Y
 * 		sequence - C9H11NO3, where C, H, N and O are names of corresponding 
 * chemical elements. The same way Tyrosine's name - Y - can be used in sequences
 * of other bio-chemical molecules, allowing molecules of any complexity to
 * be built up from smaller molecules. This hierarchical structure allows an easy folding 
 * of the molecule's isotope distribution from the distributions of elements, 
 * or other molecules it consists of.
 * 
 * @see Element
 * @see IsotopeDistribution
 * 
 * @author Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>  
 * 
 * @ingroup alphabet 
 */
class ComposedElement : public Element {
	
	public:
		/**
		 * Type of parser to parse a molecule's sequence.
		 */
		typedef AbstractMoleculeSequenceParser sequence_parser_type;
		
		/**
		 * Type of container to store elements and their abundances.
		 */
		typedef std::map<Element, unsigned, ElementSortCriteria> container;

		/**
		 * Default type of sequence, i.e. C4H8O6
		 */
		static const unsigned DEFAULT_MOLECULE_SEQUENCE_TYPE = 0;

		/**
		 * Tex-like type of sequence, i.e. C_{4}H_{8}O_{6}
		 */
		static const unsigned TEX_NOTATION_MOLECULE_SEQUENCE_TYPE = 1;
		
		/**
		 * Copy constructor.
		 */
		ComposedElement(const ComposedElement& composed_element) : 
					Element(composed_element),
					elements(composed_element.elements) {}

		/**
		 * Constructor with a given name, sequence, isotope distribution
		 * and elements molecule consists of.
		 */
		ComposedElement(const name_type& name, const name_type& sequence, 
			const isotopes_type& isotopes, const container& elements) :
					Element(name, isotopes),
					elements(elements) { this->setSequence(sequence); }
		
		/**
		 * Initializes molecule with container of elements and their abundances. Optionally
		 * an @c order in which elements should appear in molecule's sequence can be given.
		 * 
		 * @see ComposedElement(const name_type&, const Alphabet&, unsigned)
		 */
		ComposedElement(const container& elements, const std::vector<name_type>* order = NULL);

		/**
		 * Constructor with a given @c sequence, an @c alphabet that contains elements 
		 * parsed in the @c sequence and a @c sequence_type which specifies how the @c sequence to be parsed. 
		 * If no @c sequence_type is defined, @c sequence is of default type, i.e. C2H4O6N.
		 * 
		 * @throws UnknownCharacterException if no element parsed in the sequence is found
		 * 					in the alphabet
		 * @see ComposedElement(const name_type&, const Alphabet&)
		 */
		ComposedElement(const name_type& sequence, const Alphabet& alphabet,
				unsigned sequence_type = DEFAULT_MOLECULE_SEQUENCE_TYPE)
			/*throw (UnknownCharacterException)*/;
		
		/**
		 * Constructor with a given elemental composition @c decomposition that contains abundances of 
		 * elements and a set of elements @c alphabet . Index of element's abundance in the elemental 
		 * composition corresponds an index of the same element in the @c alphabet.
		 */
		ComposedElement(const std::vector<unsigned int>& decomposition, 
				const Alphabet& alphabet);

		/**
		 * Assignment operator.
		 * 
		 * @param element ComposedElement to be assigned.
		 * @return Reference to this object.
		 */			
		ComposedElement& operator =(const ComposedElement& element);

		/**
		 * Equality operator. Returns true, if a given @c element is equal
		 * to this one, false - otherwise.
		 * 
		 * @return true, if a given element is equal to this one,
		 * 		   false - otherwise
		 */
		bool operator ==(const ComposedElement& element) const;
		
		/**
		 * Inequality operator. Returns true, if a given @c element is 
		 * unequal to this one, false - otherwise.
		 * 
		 * @return true, if a given element is unequal to this one, 
		 * 		   false - otherwise.
		 */						
		bool operator !=(const ComposedElement& element) const;
		
		/**
		 * Substracting operator. If an @c element to be substracted doesn't consist of elements
		 * that are complete subset of elements in this element, than a common subset is substracted.
		 * Here are two examples:
		 * 1.Complete subset
		 *    C4H8O4 -= C2H6 -----> C2H2O4
		 * 2.Non-complete subset
		 *    C4H8O4 -= C2H10 ----> C2O4
		 *    C4H8O4 -= C2H6N ----> C2O2O4
		 * 
		 * The sequence and isotope distribution of resulting ComposedElement will be updated.
		 *
		 * @param element ComposedElement to be substracted.
		 * @return Difference between the instance of this object and @c element .
		 */
		ComposedElement& operator -=(const ComposedElement& element);

		/**
		 * Adding operator. An example C4H8O4 += C2H6N -> C6H14O4N
		 */
		ComposedElement& operator +=(const ComposedElement& element);

		/**
		 * Gets elements with their abundances.
		 * 
		 * @return Elements with their abundances.
		 */
		const container& getElements() const { return elements; }

		/**
		 * Gets abundance of the element with a given @c name in the molecule.
		 * 
		 * @param name Name of the element.
		 * @return Abudance of the element.
		 */
		container::mapped_type getElementAbundance(const name_type& name) const;


		/**
		 * Sets sequence by a given order of element names @c elements_order.
		 * If no order is given, sequence is set randomly.
		 */
		void updateSequence(const std::vector<name_type>* elements_order = NULL);
		
		/**
		 * Updates isotope distribution by folding isotope distributions of elements
		 * this composed element consists of.
		 * 
		 */
		void updateIsotopeDistribution();		

		/**
		 * Destructor.
		 */							
		virtual ~ComposedElement() {}
		
	private:
		/**
		 * Container to store elements with their abundances.
		 */
		container elements;
		
		/**
		 * Sets elements by parsing sequence. Sequence must be already set before by
		 * setSequence function. First, element's name is extracted 
		 * from the sequence. Second, an @c alphabet is searched for element with
		 * such a name. If no element is found, UnknownCharacterException is thrown. 
		 * 
		 * @param alphabet Alphabet that contains elements parsed in the sequence.
		 * @param parser Parser that defines the way how to parser the sequence.
		 * 
		 * @throws UnknownCharacterException if any error happens while parsing	molecule's sequence.
		 */
		void initializeElements(const Alphabet& alphabet, std::unique_ptr<sequence_parser_type> parser)
			/*throw (UnknownCharacterException)*/;
		
		void initializeElements(const std::vector<unsigned int>& decomposition, 
			const Alphabet& alphabet);

		/**
		 * Function object to find element by name in container.
		 */
		class FindElementByName {
			public:
				FindElementByName(const name_type& name):
					name(name) {}
				bool operator() (const container::value_type& value) const {
					return value.first.getName() == name;
				}
			private:
				name_type name;
		};
};

/**
 * Prints element to the stream @c os.
 *
 * @param os Output stream to which element is printed out.
 * @param composed_element Element to be printed out.
 */
std::ostream& operator << (std::ostream& os, const ComposedElement& composed_element);

} // namespace ims

#endif // IMS_COMPOSEDELEMENT_H
