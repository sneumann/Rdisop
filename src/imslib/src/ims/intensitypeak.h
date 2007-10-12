#ifndef IMS_INTENSITYPEAK_H
#define IMS_INTENSITYPEAK_H

#include <ostream>

namespace ims {

/**
 * Represents a peak that contains only intensity.
 *
 * @param IntensityType the intensity type
 *
 * @ingroup peaks
 */
template <typename IntensityType = double>
class IntensityPeak {
	public:
		typedef IntensityType intensity_type;
		/** Identifier for intensity property. Peaks are designed to be plugged together using
		  * multiple inheritance. This typedef allows statements like
		  * peaklist.begin<MyPeak::IntensityProperty>(); */
		struct IntensityGetter {
			typedef IntensityType value_type;
			inline static IntensityType& get(IntensityPeak<IntensityType>& p) {
				return p.getIntensityReference();
			}
			inline static const IntensityType& get(const IntensityPeak<IntensityType>& p) {
				return p.getIntensity();
			}
		};

		IntensityPeak() {}
		IntensityPeak(const IntensityPeak<IntensityType>& peak): intensity(peak.getIntensity()) {}
		IntensityPeak(intensity_type intensity): intensity(intensity) {}
		virtual ~IntensityPeak() {}
		const intensity_type& getIntensity() const { return intensity; }
		void setIntensity(intensity_type intensity) { this->intensity = intensity; }
		intensity_type getValue() const { return getIntensity();}
		void setValue(intensity_type intensity) { setIntensity(intensity); }

		bool operator==(const IntensityPeak<IntensityType>& peak) const {
			return (intensity == peak.getIntensity());
		}

		bool operator!=(const IntensityPeak<IntensityType>& peak) const {
			return !(operator==(peak));
		}

	protected:
		intensity_type& getIntensityReference() { return intensity; }
	private:
		intensity_type intensity;
};


template <typename IntensityType>
std::ostream& operator<< (std::ostream& os, const IntensityPeak<IntensityType>& peak) {
	return os << peak.getIntensity();
}

}

#endif // IMS_INTENSITYPEAK_H
