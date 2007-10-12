#ifndef IMS_INTENSITYNORMALIZERMODIFIER_H
#define IMS_INTENSITYNORMALIZERMODIFIER_H

#include <ims/modifier/modifier.h>

namespace ims {

/**
 * Normalizer for intensities. All peak intensities are divided by the maximum
 * intensity and multiplied by maxIntensity.
 *
 * @ingroup modifiers
 */
template <typename PeakListType>
class IntensityNormalizerModifier : public Modifier<PeakListType> {
	public:
		typedef typename PeakListType::peak_type::intensity_type intensity_type;
		
		IntensityNormalizerModifier(intensity_type maxIntensity=1) : maxIntensity(maxIntensity) {}
		virtual void modify(PeakListType&  peakList) const;
		virtual void print(std::ostream& os) const { os << "IntensityNormalizerModifier"; }
		virtual IntensityNormalizerModifier* clone() const {
			return new IntensityNormalizerModifier(*this);
		}
	private:
		intensity_type maxIntensity;
};

template <typename PeakListType>
void IntensityNormalizerModifier<PeakListType>::modify(PeakListType& peakList) const {
	// find out maximum intensity
	intensity_type max = 0;
	for (typename PeakListType::const_iterator it = peakList.begin(); it != peakList.end(); ++it) {
		if (it->getIntensity() > max) {
			max = it->getIntensity();
		}
	}

	// normalize
	for (typename PeakListType::iterator it = peakList.begin(); it != peakList.end(); ++it) {
		if (max > 0) {
			it->setIntensity( it->getIntensity()/max * maxIntensity);
		} else {
			it->setIntensity(maxIntensity);
		}
	}
}

} // namespace ims
#endif
