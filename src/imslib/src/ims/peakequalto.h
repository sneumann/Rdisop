#ifndef __PEAKEQUALTO
#define __PEAKEQUALTO

namespace ims {
/**
 * Class to compare one peak to a mass.
 * Returns true, if  peaks has same mass as given.
 * Should be used with the STL remove_if function.
 * For unknown reasons, it does not work with
 * the STL remove function, although similar
 * example code exists.
 *
 * @ingroup peaks
 */
template <typename PeakType, typename MassType>
class PeakEqualityToMass {
  public:
    PeakEqualityToMass(const MassType mass) : mmass(mass) {}
    bool operator() (const PeakType& p) const {
      return (this->mmass == p.getMass());
    };
    private:
      MassType mmass;
};

}
#endif
