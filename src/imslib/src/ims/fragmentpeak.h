#ifndef IMS_FRAGMENTPEAK_H
#define IMS_FRAGMENTPEAK_H

#include <ostream>

#include <ims/masspeak.h>

namespace ims {

/**
 * Represents a peak that was originated by fragmenting a sequence.
 *
 * @ingroup peaks
 */
template <typename MassType = double>
class FragmentPeak : public MassPeak<MassType> {
	public:
		typedef typename MassPeak<MassType>::mass_type mass_type;

		// TODO getStart() vs getFragmentStart()
		struct FragmentStartGetter {
			typedef size_t value_type;
			inline static value_type& get(FragmentPeak<MassType>& p) {
				return p.getStartReference();
			}
			inline static const value_type& get(const FragmentPeak<MassType>& p) {
				return p.getStart();
			}
		};
		struct FragmentLengthGetter {
			typedef size_t value_type;
			inline static value_type& get(FragmentPeak<MassType>& p) {
				return p.getLengthReference();
			}
			inline static const value_type& get(const FragmentPeak<MassType>& p) {
				return p.getLength();
			}
		};
		struct MiscleavageCountGetter {
			typedef size_t value_type;
			inline static value_type& get(FragmentPeak<MassType>& p) {
				return p.getMiscleavageCountReference();
			}
			inline static const value_type& get(const FragmentPeak<MassType>& p) {
				return p.getMiscleavageCount();
			}
		};

		/** Construct FragmentPeak initializing all fields to 0. */
		FragmentPeak() : MassPeak<MassType>(0), start(0), length(0), miscleavage_count(0) {}

		/** Copy Constructor. */
		FragmentPeak(const FragmentPeak& peak) :
			MassPeak<MassType>(peak), start(peak.start),
			length(peak.length), miscleavage_count(peak.miscleavage_count) {}

		/** TODO can this be done bette? need to be able to initialize a FragmentPeak from
		 * a MassPeak, especially for the NoiseModifier */
		FragmentPeak(const MassPeak<MassType>& mass_peak) :
				MassPeak<MassType>(mass_peak), start(0), length(0), miscleavage_count(0) {}

		/** Constructor.
		 * @param mass Mass of fragment, i.e. sum of masses of all characters.
		 * @param start Start position of fragment in the sequence.
		 * @param length Length of fragment (in characters).
		 * @param miscleavage_count see getMiscleavageCount for details.
		 */
		FragmentPeak(mass_type mass, size_t start, size_t length, size_t miscleavage_count=0) :
			MassPeak<MassType>(mass),
			start(start),
			length(length),
			miscleavage_count(miscleavage_count)
		{
		}

		const size_t& getLength() const { return length; }
		void setLength(size_t length) { this->length=length; }
		const size_t& getStart() const { return start; }
		void setStart(size_t start) { this->start=start; }
		/** Returns the number of cleavage characters which were missed.
		 * Missing of cleavage characters is supported by PMFFragmenter to simulate
		 * incomplete cleavage reactions.
		 */
		const size_t& getMiscleavageCount() const { return miscleavage_count; }
		void setMiscleavageCount(size_t miscleavage_count) { this->miscleavage_count=miscleavage_count; }

		bool operator==(const FragmentPeak<MassType>& peak) const {
			return (MassPeak<MassType>::operator==(peak) && start==peak.start
				&& length==peak.length && miscleavage_count==peak.miscleavage_count);
		}
		bool operator!=(const FragmentPeak<MassType>& peak) const {
			return !(operator==(peak));
		}

	protected:
		size_t& getLengthReference() { return length; }
		size_t& getStartReference() { return start; }
		size_t& getMiscleavageCountReference() { return miscleavage_count; }
	private:
		size_t start;
		size_t length;
		size_t miscleavage_count;
};

template <typename MassType>
std::ostream& operator<< (std::ostream& os, const FragmentPeak<MassType>& peak) {
	return os << "(" << peak.getMass() << " " << peak.getStart() << " " << peak.getLength()
		<< " " << peak.getMiscleavageCount() << ")";
}

}

#endif // IMS_FRAGMENTPEAK_H
