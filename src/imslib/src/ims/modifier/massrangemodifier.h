#ifndef IMS_MASSRANGEMODIFIER_H
#define IMS_MASSRANGEMODIFIER_H

#include <ostream>

#include <ims/modifier/modifier.h>

namespace ims {

/**
 * Modifier that erases all peaks with a mass below @c minMass or above @c maxMass
 *
 * @ingroup modifiers
 */
template <typename PeakListType>
class MassRangeModifier : public Modifier<PeakListType> {
	public:
		typedef typename PeakListType::peak_type::mass_type mass_type;
		MassRangeModifier(mass_type minMass=0, mass_type maxMass=std::numeric_limits<mass_type>::max())
			: minMass(minMass), maxMass(maxMass) {}

		virtual void modify(PeakListType& peakList) const;
		virtual void print(std::ostream& os) const {
			os << "MassRangeModifier(min=" << minMass << ", max=" << maxMass << ")";
		}
		virtual MassRangeModifier* clone() const {
			return new MassRangeModifier(*this);
		}

	private:
		class MassOutOfRangePredicate {
			public:
				MassOutOfRangePredicate(mass_type minMass, mass_type maxMass) : minMass(minMass), maxMass(maxMass) { }
				bool operator() (const typename PeakListType::peak_type& peak) const {
					return peak.getMass() < minMass || peak.getMass() > maxMass;
				}
			private:
				mass_type minMass, maxMass;
			
		};
		mass_type minMass;
		mass_type maxMass;
};



template <typename PeakListType>
void MassRangeModifier<PeakListType>::modify(PeakListType& peakList) const {
	typename PeakListType::iterator it = std::remove_if(peakList.begin(), peakList.end(), MassOutOfRangePredicate(minMass, maxMass) );
	peakList.erase(it, peakList.end());
}

}
#endif
