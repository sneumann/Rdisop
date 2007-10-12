#ifndef IMS_NOISEMODIFIER_H
#define IMS_NOISEMODIFIER_H

#include <vector>
#include <cmath>
//FIXME do we need GSL
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <ims/masspeak.h>
#include <ims/modifier/modifier.h>

#include <cassert>

namespace ims {
/**
 * A modifier that
 *  - adds noise to peaks of a peaklist,
 *  - removes some peaks,
 *  - and adds some peaks.
 * With this, one might try to disturb a predicted spectrum
 * to simulate measurement errors and additional/missing peaks.
 * @ingroup modifiers
 */
template <typename PeakListType>
class NoiseModifier : public Modifier<PeakListType> {
	public:
		typedef typename PeakListType::peak_type::mass_type mass_type;
		NoiseModifier(double width, double perc_miss, double perc_add, mass_type maxmass);
		virtual void modify(PeakListType&  peakList) const;
		virtual void print(std::ostream& os) const { os << "NoiseModifier"; }
		virtual void setWidth(double width) {this->width = width;}
		virtual void setAdd(double add) {this->perc_add = add;}
		virtual void setMiss(double miss) {this->perc_miss = miss;}
		virtual void setMaxMass(mass_type maxmass ) {this->maxmass = maxmass;}
		virtual double getWidth() const { return this->width;}
		virtual double getAdd() const { return this->perc_add;}
		virtual double getMiss() const { return this->perc_miss;}
		virtual mass_type getMaxMass() const { return this->maxmass;}
		virtual NoiseModifier* clone() const {
			return new NoiseModifier(*this);
		}

	private:
		double width;
		double perc_miss;
		double perc_add;
		mass_type maxmass;
};

/**
  * Creates a new NoiseModifier.
  * @param width Width of uniform r.v. added to each mass.
  * @param perc_miss A value between 0 and 1 giving the probability for each peak to be deleted.
  * @param perc_add A value between 0 and 1. perc_add*(# peaks) new peaks are created.
  * @param maxmass Maximal mass for newly added peaks. Mass is drawn uniformly from 0..maxmass.
  */
template <typename PeakListType>
NoiseModifier<PeakListType>::NoiseModifier(double width, double perc_miss, double perc_add, mass_type maxmass) :
	width(width),
	perc_miss(perc_miss),
	perc_add(perc_add),
	maxmass(maxmass)
{
	assert(width>=0.0);
	assert(perc_miss>=0.0);
	assert(perc_add>=0.0);
	assert(maxmass>=0.0);
}

/**
  * Modifies the given peak list by "shaking" each mass
  * with a uniform noise with given width.
  * For each peak, a decision is made whether to delete it or
  * not, deletion occurs with probability perc_miss.
  * Afterwards, approximately perc_add*(peak list size) new peaks
  * are created, each having a random mass uniformly between
  * 0 and maxmass.
  * The peak list is then resized to its new size.
  * @param peakList The peak list to be modified.
  */
template <typename PeakListType>
void NoiseModifier<PeakListType>::modify(PeakListType& peakList) const {
	// shake masses a little bit ;)
	double add;
	double sign;
	gsl_rng_default_seed = (unsigned) time(NULL); //FIXME Mitch fragen
	gsl_rng * r = gsl_rng_alloc(gsl_rng_taus);
	for(size_t i=0;i<peakList.size();i++) {
		//add = gsl_ran_gaussian (r, sqrt(width));
		// mass difference
		add = gsl_rng_uniform(r) * width;
		// sign of mass difference
		sign = gsl_rng_uniform(r);
		if(sign<=0.5){
			add = -add;
		}
		peakList[i].setMass(peakList[i].getMass() + (mass_type)add);
	}
	// now delete some peaks
	for(typename PeakListType::iterator it=peakList.begin(); it<peakList.end(); ++it) {
		double randy = gsl_rng_uniform(r);
		if(randy<perc_miss) {
			peakList.erase(it);
		}
	}
	// add some peaks
	size_t new_size = (unsigned)(peakList.size()*perc_add);
	for(size_t i=0; i<new_size; i++) {
		double randy = gsl_rng_uniform(r)*maxmass;
		//peakList.push_back(ims::MassPeak<mass_type>(randy));
		// FIXME old code had 1.0 in the constructor parameters
		// (for the intensity)
		// I'm not sure how to deal with the problem that this
		// modifier may be used for different peak_types which
		// have different constructors.
		peakList.push_back(typename PeakListType::peak_type((mass_type)randy));

	}
}

}
#endif
