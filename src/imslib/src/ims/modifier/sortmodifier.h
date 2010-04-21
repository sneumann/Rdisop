#ifndef IMS_SORTMODIFIER_H
#define IMS_SORTMODIFIER_H

#include <algorithm>
#include <ims/peakcompare.h>
#include <ims/modifier/modifier.h>

namespace ims {

/**
 * Peak list modifier which sorts peaks by masses in ascending order.
 * @ingroup modifiers
 */
template <typename PeakListType>
class SortModifier : public Modifier<PeakListType> {
	public:
		SortModifier() {}
		virtual void modify(PeakListType& peakList) const;
		virtual void print(std::ostream& os) const { os << "SortModifier"; }
		virtual SortModifier* clone() const {
			return new SortModifier(*this);
		}
};


template <typename PeakListType>
void SortModifier<PeakListType>::modify(PeakListType& peakList) const {
	//FIXME man sollte den peak fragen koennen über welche Funktion er verglichen werden möchte
	std::sort(peakList.begin(), peakList.end(), PeakCompareByMass<typename PeakListType::peak_type>());
}

}
#endif
