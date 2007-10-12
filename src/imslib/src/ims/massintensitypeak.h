#ifndef IMS_MASSINTENSITYPEAK_H
#define IMS_MASSINTENSITYPEAK_H

#include <ims/masspeak.h>
#include <ims/intensitypeak.h>

namespace ims {

/**
 * A peak with a mass and an intensity.
 *
 * @param MassType Type of the mass value. Most likely only numeric types make sense here.
 * @param IntensityType Type of the intensity value.
 *
 * @ingroup peaks
 */
template <typename MassType, typename IntensityType>
class MassIntensityPeak : public MassPeak<MassType>,
                          public IntensityPeak<IntensityType> {
	public:
		MassIntensityPeak() :
			MassPeak<MassType>(),
			IntensityPeak<IntensityType>() {}

		MassIntensityPeak(const MassIntensityPeak<MassType,IntensityType>& peak) :
			MassPeak<MassType>(peak),
			IntensityPeak<IntensityType>(peak) {}

		MassIntensityPeak(MassType mass, IntensityType intensity = 1.0) : // FIXME
			MassPeak<MassType>(mass),
			IntensityPeak<IntensityType>(intensity) {}

		MassIntensityPeak<MassType,IntensityType>& operator= (const MassIntensityPeak<MassType,IntensityType>& peak) {
			if (this != &peak) {
				MassPeak<MassType>::operator=(peak);
				IntensityPeak<IntensityType>::operator=(peak);
			}
			return *this;
		}

		bool operator==(const MassIntensityPeak<MassType,IntensityType>& peak) const {
			return (MassPeak<MassType>::operator==(peak) &&
			        IntensityPeak<IntensityType>::operator==(peak));
		}

		bool operator!=(const MassIntensityPeak<MassType,IntensityType>& peak) const {
			return !(operator==(peak));
		}
};

template <typename MassType, typename IntensityType>
std::ostream& operator<< (std::ostream& os, const MassIntensityPeak<MassType,IntensityType>& peak) {
	return os << "(" << peak.getMass() << " " << peak.getIntensity() << ")";
}

}

#endif // IMS_MASSINTENSITYPEAK_H
