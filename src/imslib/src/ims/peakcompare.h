#ifndef IMS_PEAKCOMPARE_H
#define IMS_PEAKCOMPARE_H


namespace ims {
/**
 * Class to compare two peaks by mass.
 * Should be used with the STL sort function
 *
 * @ingroup peaks
 */
template <typename PeakType>
class PeakCompareByMass {
	public:
		bool operator() (const PeakType& p1, const PeakType& p2) const {
			return p1.getMass() < p2.getMass();
		}
};

}
#endif
