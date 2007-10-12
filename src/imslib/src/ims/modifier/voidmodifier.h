#ifndef IMS_VOIDMODIFIER_H
#define IMS_VOIDMODIFIER_H

#include <ims/modifier/modifier.h>

namespace ims {

/**
 * Implements a modifier class that does nothing. Only for
 * testing purposes.
 *
 * @ingroup modifiers
 */
template <typename PeakListType>
class VoidModifier : public Modifier<PeakListType> {
	public:
		/**
		* Returns unchanged peaklist.
		* @param peakList A peaklist
		*/
		virtual void modify(PeakListType& peakList) const { (void)peakList; }
		virtual void print(std::ostream& os) const { os << "VoidModifier"; }
		virtual VoidModifier* clone() const {
			return new VoidModifier(*this);
		}
};

}
#endif
