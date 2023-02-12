/**
 * linearpointsetmatcher.h
 *
 * Author: Tobias Marschall <Tobias.Marschall@CeBiTec.Uni-Bielefeld.DE>
 */

#include <cmath>
#include <limits>

#include <ims/transformation.h>
#include <ims/calib/linearpointsetmatcher.h>

namespace ims {

bool RepresentativeScale::operator<(const RepresentativeScale &y) const {
	if (scale == y.scale)
		return end < y.end;
	else
		return scale < y.scale;
};

void LinearPointSetMatcher::setAbsLimit(double limit) {
	if (abslimit<0) return;
	abslimit=limit;
}

void LinearPointSetMatcher::unsetAbsLimit() {
	abslimit=std::numeric_limits<float>::infinity();
}

void LinearPointSetMatcher::setTranslationInterval(double min, double max){
	if (min > max) return;
	this->maxtranslation=max;
	this->mintranslation=min;
}

void LinearPointSetMatcher::unsetTranslationInterval() {
	this->mintranslation = -std::numeric_limits<float>::infinity();
	this->maxtranslation = std::numeric_limits<float>::infinity();
}

std::pair<double,double> LinearPointSetMatcher::getTranslationInterval() const {
	return std::pair<double,double>(mintranslation,maxtranslation);
}

void LinearPointSetMatcher::setScaleInterval(double min, double max) {
	if (min > max) return;
	this->maxscale=max;
	this->minscale=min;
}

void LinearPointSetMatcher::unsetScaleInterval() {
	this->minscale = -std::numeric_limits<float>::infinity();
	this->maxscale = std::numeric_limits<float>::infinity();
}

std::pair<double,double> LinearPointSetMatcher::getScaleInterval() const {
	return std::pair<double,double>(minscale,maxscale);
}

double LinearPointSetMatcher::getAbsLimit() const {
	return abslimit;
}

double LinearPointSetMatcher::getEpsilon() const {
	return epsilon;
}

bool LinearPointSetMatcher::one2One() const {
	return oneToOne;
}

bool LinearPointSetMatcher::restrictOne2One() const {
	return restrict_oneToOne;
}

// TODO remove this
void LinearPointSetMatcher::swap(double& d1, double& d2) {
	double h=d1;
	d1=d2;
	d2=h;
}

std::unique_ptr<std::map<int,int> > LinearPointSetMatcher::getMapping() const {
	if (results.mapping.get() == 0) {
		return std::unique_ptr<std::map<int,int> >(new std::map<int,int>()); // TODO throw sth. instead
	} else {
		// aaargh, this syntax is awful, i hate it, hate it, hate it...
		return std::unique_ptr<std::map<int,int> >(new std::map<int,int>(*(results.mapping)));
	}
}

LinearTransformation LinearPointSetMatcher::getTransformation() const {
	return LinearTransformation(results.bestscale, results.besttranslation);
}

} // namespace ims
