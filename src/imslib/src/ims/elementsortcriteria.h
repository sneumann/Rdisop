#ifndef IMS_ELEMENTSORTCRITERIA_H
#define IMS_ELEMENTSORTCRITERIA_H

#include <ims/element.h>

namespace ims {

class ElementSortCriteria {
	public:
		bool operator()(const Element& el1,
						const Element& el2) const {
			return el1.getSequence() < el2.getSequence();
		}
};

} // namespace

#endif // IMS_ELEMENTSORTCRITERIA_H
