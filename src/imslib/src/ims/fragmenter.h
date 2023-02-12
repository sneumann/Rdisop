#ifndef IMS_FRAGMENTER_H
#define IMS_FRAGMENTER_H

#include <vector>
#include <memory>

#include <ims/peaklist.h>
#include <ims/fragmentpeak.h>
#include <ims/modifier/modifier.h>

namespace ims {

/**
 * An abstract base class for fragmenters.
 *
 * A Fragmenter roughly simulates the following process:
 * - Cut up one biological sequence (e.g. DNA or RNA) into
 *   fragments (substrings).
 * - Obtain a mass spectrum of the fragments.
 * - Detect peaks in the spectrum.
 *
 * So the input is one sequence, the output a list of peaks (PeakList).
 *
 * By default, the peaklist is returned in the way the fragmentation
 * algorithm provides it. That is, the masses are in general not sorted,
 * there may be duplicate peaks (peaks having the same mass), and there
 * is no noise. If any of this is needed, use setModifier() to provide
 * a Modifier which will alter the peaklist before it gets returned.
 *
 * @param MassType Type of masses in the peaklist. Only a floating-point
 *   type makes sense here.
 */
template <typename MassType>
class Fragmenter {
public:
	typedef FragmentPeak<MassType> peak_type;
	typedef PeakList<peak_type> peaklist_type;

	Fragmenter() : modifier(0) { }
	Fragmenter(const Fragmenter<MassType>& fragmenter) : modifier(0) {} // FIXME 0

	/**
	 * Computes the peaklist from a sequence.
	 * @param peaklist Peaklist to store results in. Former contents is deleted.
	 * @param sequence Sequence to calculate fragments from.
	 */
	virtual void predictSpectrum(peaklist_type* peaklist, const std::string& sequence) = 0;

	/**
	 * Sets a new Modifier. Assumes ownership.
	 * @param modifier new Modifier
	 */
	virtual void setModifier(std::unique_ptr<Modifier<peaklist_type> > modifier) {
		this->modifier = modifier;
	}

	virtual ~Fragmenter() { }

protected:
	std::unique_ptr<Modifier<peaklist_type> > modifier;
};

}
#endif
