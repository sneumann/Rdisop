#ifndef IMS_SHIFTMODIFIER_H
#define IMS_SHIFTMODIFIER_H

#include <ims/modifier/modifier.h>

namespace ims {
/**
 * Shift all masses by "shift"
 * @ingroup modifiers
 */
template <typename PeakListType>
class ShiftModifier : public Modifier<PeakListType> {
	public:
		typedef typename PeakListType::peak_type::mass_type mass_type;
		ShiftModifier(mass_type shift): shift(shift) {}
		virtual void modify(PeakListType& peakList) const;
		virtual void print(std::ostream& os) const { os << "ShiftModifier(shift=" << shift << ")"; }
		virtual ShiftModifier* clone() const {
			return new ShiftModifier(*this);
		}

	private:
		mass_type shift;
};

template <typename PeakListType>
void ShiftModifier<PeakListType>::modify(PeakListType& peakList) const {
	for(typename PeakListType::iterator it = peakList.begin(); it != peakList.end(); ++it){
		it->setMass(it->getMass() + shift);
	}
}

}
#endif
