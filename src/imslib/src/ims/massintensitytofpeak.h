#ifndef IMS_MASSINTENSITYTOFPEAK_H
#define IMS_MASSINTENSITYTOFPEAK_H

#include <ims/masspeak.h>
#include <ims/intensitypeak.h>
#include <ims/tofpeak.h>

namespace ims {

/**
 * A peak with a mass, an intensity, and a TOF value.
 *
 * @ingroup peaks
 */
template <typename MassType, typename IntensityType, typename TOFType>
class MassIntensityTOFPeak : public MassPeak<MassType>,
                             public IntensityPeak<IntensityType>,
                             public TOFPeak<TOFType> {
	public:
		typedef MassType mass_type;
		typedef IntensityType intensity_type;
		typedef TOFType tof_type;

		MassIntensityTOFPeak() : MassPeak<MassType>(), IntensityPeak<IntensityType>(), TOFPeak<TOFType>() {}

		MassIntensityTOFPeak(const MassIntensityTOFPeak<MassType, IntensityType, TOFType>& peak) :
			MassPeak<MassType>(peak), IntensityPeak<IntensityType>(peak), TOFPeak<TOFType>(peak) {}

		MassIntensityTOFPeak(MassType mass, IntensityType intensity, TOFType tof):
			MassPeak<MassType>(mass), IntensityPeak<IntensityType>(intensity), TOFPeak<TOFType>(tof) {}

		MassIntensityTOFPeak<MassType, IntensityType, TOFType>& operator= (const MassIntensityTOFPeak<MassType, IntensityType, TOFType>& peak) {
			if (this != &peak) {
				MassPeak<MassType>::operator=(peak);
				IntensityPeak<IntensityType>::operator=(peak);
				TOFPeak<TOFType>::operator=(peak);
			}
			return *this;
		}

		bool operator==(const MassIntensityTOFPeak<MassType, IntensityType, TOFType>& peak) const {
			return (MassPeak<MassType>::operator==(peak) &&
			        IntensityPeak<IntensityType>::operator==(peak) &&
			        TOFPeak<TOFType>::operator==(peak));
		}

		bool operator!=(const MassIntensityTOFPeak<MassType, IntensityType, TOFType>& peak) const {
			return !(operator==(peak));
		}
};

template <typename MassType, typename IntensityType, typename TOFType>
std::ostream& operator<< (std::ostream& os, const MassIntensityTOFPeak<MassType,IntensityType,TOFType>& peak) {
	return os << "(" << peak.getMass() << " " << peak.getIntensity() << " " << peak.getTOF() << ")";
}


}

#endif // IMS_MASSINTENSITYTOFPEAK_H
