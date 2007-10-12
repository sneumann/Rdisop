#ifndef IMS_UNIFICATIONMODIFIER_H
#define IMS_UNIFICATIONMODIFIER_H

#include <cmath>

#include <ims/modifier/modifier.h>

namespace ims {
/**
 * Modifier that removes all peaks with duplicate masses. The peaklist must
 * already be sorted.
 *
 * @see SortModifier
 * @ingroup modifiers
 */
template <typename PeakListType>
class UnificationModifier : public Modifier<PeakListType> {
	public:
		UnificationModifier(double epsilon = 1e-6) : epsilon(epsilon) { }
		
		virtual void modify(PeakListType&  peakList) const;
		
		virtual void print(std::ostream& os) const { os << "UnificationModifier"; }
		
		virtual UnificationModifier* clone() const {
			return new UnificationModifier(*this);
		}

	private:
		double epsilon;
};


template <typename PeakListType>
void UnificationModifier<PeakListType>::modify(PeakListType& peakList) const {
	for (typename PeakListType::iterator it = peakList.begin(); (it+1) < peakList.end(); ++it) {
		if (fabs(it->getMass() - (it+1)->getMass()) <= epsilon) {
			// FIXME addReference doesn't exist anymore
			// it->addReference( (it+1)->getReferences() );
			
			// TODO adjust intensity? 
			peakList.erase(it+1);
			--it;
		}
	}
}

} // namespace ims
#endif
