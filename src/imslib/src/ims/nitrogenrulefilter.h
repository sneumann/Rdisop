#ifndef IMS_NITROGENRULEFILTER_H
#define IMS_NITROGENRULEFILTER_H

#include <ims/composedelement.h>

namespace ims {

class NitrogenRuleFilter {
	public:
		static bool isMoleculeValid(const ComposedElement& molecule);
};


} // namespace ims

#endif // IMS_NITROGENRULEFILTER_H
