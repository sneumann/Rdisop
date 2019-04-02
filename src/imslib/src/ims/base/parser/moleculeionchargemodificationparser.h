#ifndef __MOLECULEIONCHARGEMODIFICATIONPARSER_H
#define __MOLECULEIONCHARGEMODIFICATIONPARSER_H

#include <map>
#include <vector>
#include <string>

#include <ims/base/parser/../exception/unknowncharacterexception.h>

namespace ims {

class MoleculeIonChargeModificationParser {
	public:
		typedef unsigned int mass_factor_type;
		typedef unsigned int charge_type;

		typedef std::string ion_name_type;
		typedef ion_name_type::size_type size_type;
		typedef unsigned int elements_amount_type;
		typedef std::map<ion_name_type, elements_amount_type> ion_elements_type;

		struct Ion {
			ion_elements_type elements;
			bool is_ion_added;
			bool is_ion_cation;
		};

		typedef Ion ion_type;
		typedef std::vector<ion_type> ions_type;
		
		static const char MOLECULE_SYMBOL = 'M';
		static const char CATION_CHARGE_SYMBOL = '+';
		static const char ANION_CHARGE_SYMBOL = '-';
				
		MoleculeIonChargeModificationParser() : 
			mass_factor(1),		
			charge(1) {}
		
		MoleculeIonChargeModificationParser(const ion_name_type& sequence) 
		    /*throw (UnknownCharacterException)*/;
		
		MoleculeIonChargeModificationParser(const MoleculeIonChargeModificationParser& parser) :
			mass_factor(parser.mass_factor),
			charge(parser.charge),
			ions(parser.ions) {}
		
		MoleculeIonChargeModificationParser& operator =(const MoleculeIonChargeModificationParser& parser);
		
		void parse(const ion_name_type& sequence) /*throw (UnknownCharacterException)*/;

		mass_factor_type getMassFactor() const { return mass_factor; }		
		charge_type getCharge() const { return charge; }
		const ions_type& getIons() const { return ions; }	
	private:
		mass_factor_type mass_factor;
		charge_type charge;
		ions_type ions;
		
		void clear();
};

} // namespace ims

#endif // __DEFAULTMOLECULESEQUENCEPARSER_H
