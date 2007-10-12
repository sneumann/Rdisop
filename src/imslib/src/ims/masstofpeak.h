#ifndef IMS_MASSTOFPEAK_H
#define IMS_MASSTOFPEAK_H

#include <ims/masspeak.h>
#include <ims/tofpeak.h>

namespace ims {

/** @ingroup peaks */
template <typename MassType, typename TOFType>
class MassTOFPeak : public MassPeak<MassType>,
                    public TOFPeak<TOFType> {
	public:

		MassTOFPeak() : MassPeak<MassType>(),
						TOFPeak<TOFType>() {}

		MassTOFPeak(const MassTOFPeak<MassType,TOFType>& peak) :
						MassPeak<MassType>(peak),
						TOFPeak<TOFType>(peak) { }

		MassTOFPeak(MassType mass, TOFType tof):
						MassPeak<MassType>(mass),
						TOFPeak<TOFType>(tof) { }

		MassTOFPeak<MassType, TOFType>& operator= (const MassTOFPeak<MassType, TOFType>& peak) {
			if (this != &peak) {
				MassPeak<MassType>::operator=(peak);
				TOFPeak<TOFType>::operator=(peak);
			}
			return *this;
		}

		bool operator==(const MassTOFPeak<MassType, TOFType>& peak) const {
			return (MassPeak<MassType>::operator==(peak) &&
			        TOFPeak<TOFType>::operator==(peak));
		}

		bool operator!=(const MassTOFPeak<MassType, TOFType>& peak) const {
			return !(operator==(peak));
		}
};

template <typename MassType, typename TOFType>
std::ostream& operator<< (std::ostream& os, const MassTOFPeak<MassType,TOFType>& peak) {
	return os << "(" << peak.getMass() << " " << peak.getTOF() << ")";
}

}

#endif // IMS_MASSTOFPEAK_H
