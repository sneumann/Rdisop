#ifndef IMS_MODIFIER_H
#define IMS_MODIFIER_H

#include <ostream>

namespace ims {

/**
 * The Modifier interface class.
 * A Modifier is a class that changes a peaklist.
 * Examples would be computation of A/B/C/X/Y/Z series
 * peaks from a original predicted MS/MS peaklist, the 
 * addition and deletion of peaks as well as mass changes, 
 * the simulation of mass modification like H2O-losses etc.
 *
 * @ingroup modifiers
 */
template <typename PeakListType>
class Modifier {
	public:
		virtual ~Modifier() {}
		/**
		 * Modifies the given peaklist.
		 * @param peakList peak list to be modified.
		 */
		virtual void modify(PeakListType& peakList) const = 0;

		/**
		 * Prints this modifier.
		 */
		virtual void print(std::ostream& os) const = 0;
		
		/**
		 * Constructs an exact copy. Calls new(), object must be deleted by caller.
		 */
		virtual Modifier* clone() const = 0;
};

template <typename PeakListType>
std::ostream& operator<< (std::ostream& os, const Modifier<PeakListType>& modifier) {
	modifier.print(os);
	return os;
}

}
#endif
