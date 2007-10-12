#ifndef __PEAKEQUALBY
#define __PEAKEQUALBY


namespace ims {
/**
 * Class to compare two peaks by mass.
 * Returns true, if two peaks have same mass.
 * Should be used with the STL unique function
 *
 * @ingroup peaks
 */
template <typename PeakType>
class PeakEqualityByMass {
  public:
    bool operator() (const PeakType& p1, PeakType& p2) const {
      return p1.getMass() == p2.getMass();
    };
};

}
#endif
