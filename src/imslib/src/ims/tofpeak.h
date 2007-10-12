#ifndef IMS_TOFPEAK_H
#define IMS_TOFPEAK_H

namespace ims {

/**
 * Represents a peak that contains a "time-of-flight" property
 * (a certain physical property of molecules in mass spectrometer).
 * @param TOFType the "time-of-flight" property type
 *
 * @ingroup peaks
 */
template <typename TOFType = double>
class TOFPeak {
	public:
		typedef TOFType tof_type;
		/** Identifier for tof property. Peaks are designed to be plugged together using
		  * multiple inheritance. This typedef allows statements like
		  * peaklist.begin<MyPeak::TOFGetter>(); */
		struct TOFGetter {
			typedef TOFType value_type;
			inline static TOFType& get(TOFPeak<TOFType>& p) { return p.getTOFReference(); }
			inline static const TOFType& get(const TOFPeak<TOFType>& p) { return p.getTOF(); }
		};

		TOFPeak() {}
		TOFPeak(const TOFPeak<TOFType>& peak): tof(peak.getTOF()) {}
		TOFPeak(tof_type tof): tof(tof) {}
		const tof_type& getTOF() const { return tof; }
		void setTOF(tof_type tof) { this->tof = tof; }

		bool operator==(const TOFPeak<TOFType>& peak) const {
			return (tof == peak.getTOF());
		}

		bool operator!=(const TOFPeak<TOFType>& peak) const {
			return !(operator==(peak));
		}
	protected:
		tof_type& getTOFReference() { return tof; }
	private:
		tof_type tof;
};


template <typename TOFType>
std::ostream& operator<< (std::ostream& os, const TOFPeak<TOFType>& peak) {
	return os << peak.getTOF();
}

}

#endif // IMS_TOFPEAK_H
