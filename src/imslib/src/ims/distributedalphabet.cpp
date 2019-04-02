/**
 * distributedalphabettest.cpp
 *
 * Author: Anton Pervukhin <Anton.Pervukhin@CeBiTec.Uni-Bielefeld.DE>
 */
#include <ims/utils/math.h>
#include <ims/distributedalphabet.h>
#include <ims/base/parser/distributedalphabettextparser.h>

namespace ims {

const double DistributedAlphabet::DISTRIBUTION_PRECISION = 0.001;


bool DistributedAlphabet::isDistributionCorrect() const{
	return fabs(getDistributionSum() - 1) < DISTRIBUTION_PRECISION;
}


void DistributedAlphabet::normalizeDistribution() {
	if (!isDistributionCorrect() && elementsDistribution.size() > 0) {
		double scale = 1 / getDistributionSum();
		for (iterator begin = elementsDistribution.begin(),
		end = elementsDistribution.end(); begin != end; ++begin) {
			begin->second *= scale;
		}
	}
}


double DistributedAlphabet::getProbability(const name_type& name) const {
	const_iterator position = elementsDistribution.find(name);
	return (position != elementsDistribution.end())? position->second: 0.0;
}


double DistributedAlphabet::getProbability(size_type index) const {
	return getProbability(this->getName(index));
}


void DistributedAlphabet::setProbability(const name_type& name, 
														double probability) {
	if (this->hasName(name)) {
		elementsDistribution[name] = probability;
	}
}


void DistributedAlphabet::push_back(const name_type& name, mass_type value, 
														double probability) {
	Alphabet::push_back(name, value);
	this->setProbability(name, probability);
}


void DistributedAlphabet::load(const std::string& name) /*throw (IOException)*/ {
	// here instead of explicit calling of corresponding parser 
	// implemntation constructor
	// one should use factory later on, something like this:
	// this->load(name, ComponentsManager.getParsersCreator().
	// getDistributedAlphabetDefaultParser());
	this->load(name, new DistributedAlphabetTextParser);
}


void DistributedAlphabet::load(const std::string& name, 
					AlphabetParser<>* parser) /*throw (IOException)*/ {
	Alphabet::load(name, parser);
	DistributedAlphabetParser<> *castedParser = 
					dynamic_cast< DistributedAlphabetParser<>* >(parser);
	if (castedParser != NULL) {
		this->setDistribution(castedParser->getElementsDistribution());
		this->normalizeDistribution();
	}
}

double DistributedAlphabet::getDistributionSum() const {
	double sum = 0.0;
	for(const_iterator begin = elementsDistribution.begin(),
	end = elementsDistribution.end(); begin != end; ++begin) {
		sum += begin->second;
	}
	return sum;
}


void DistributedAlphabet::setDistribution(
							const container& elementsDistribution) {
	this->elementsDistribution = elementsDistribution;
}

} // namespace ims
