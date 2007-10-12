/**
 * Author: Anton Pervukhin <Anton.Pervukhin@minet.uni-jena.de>
 */

#include <ims/nitrogenrulefilter.h>

namespace ims {

bool NitrogenRuleFilter::isMoleculeValid(const ComposedElement& molecule) {

	// NOTE: implementation is not yet ideal, since we explicitly use knowledge, 
	// that hydrogen has valency 1, nitrogen and phosphor - 3, and carbon - 4.
	// The better implementation would be to go through every element in composed element
	// and collect the number of those with valency 1, 3 and 4.
	// For this one would need a member variable that represents valency in a class that
	// represents a simple element (atom), which is not yet the case.
	int n_valencies1 = static_cast<int>(molecule.getElementAbundance("H"));
	int n_valencies3 = static_cast<int>(molecule.getElementAbundance("N")) +
				static_cast<int>(molecule.getElementAbundance("P"));
	int n_valencies4 = static_cast<int>(molecule.getElementAbundance("C"));
	
	return ((n_valencies1 - n_valencies3) % 2 == 0) &&			// filters if 2*DU is even
		((n_valencies3 + 2 * n_valencies4 + 2) >= n_valencies1);	// filters if DU > 0
}

} // namespace ims
