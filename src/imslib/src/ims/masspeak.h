#ifndef IMS_MASSPEAK_H
#define IMS_MASSPEAK_H

#include <ostream>

namespace ims {

/**
 * Represents a peak that contains only a mass.
 *
 * @param MassType the mass type
 *
 * @ingroup peaks
 */
template <typename MassType = double>
class MassPeak {
	public:
		typedef MassType mass_type;
		/** Identifier for mass property. Peaks are designed to be plugged together using
		  * multiple inheritance. This typedef allows statements like
		  * peaklist.begin&lt;MyPeak::MassProperty&gt;(); */
		struct MassGetter {
			typedef MassType value_type;
			inline static value_type& get(MassPeak<MassType>& p) { return p.getMassReference(); }
			inline static const value_type& get(const MassPeak<MassType>& p) { return p.getMass(); }
		};

		MassPeak() {}
		MassPeak(const MassPeak<MassType>& peak): mass(peak.mass) {}
		MassPeak(mass_type mass): mass(mass) {}
		const mass_type& getMass() const { return mass; }
		void setMass(mass_type mass) { this->mass = mass; }

		bool operator==(const MassPeak<MassType>& peak) const {
			return mass == peak.mass;
		}

		bool operator!=(const MassPeak<MassType>& peak) const {
			return !(operator==(peak));
		}
	protected:
		mass_type& getMassReference() { return mass; }
	private:
		mass_type mass;
};


template <typename MassType>
std::ostream& operator<< (std::ostream& os, const MassPeak<MassType>& peak) {
	return os << peak.getMass();
}

}

#endif // IMS_MASSPEAK_H
