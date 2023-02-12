#ifndef IMS_RANDOMSEQUENCEGENERATOR_H
#define IMS_RANDOMSEQUENCEGENERATOR_H

#include <memory>

#include <ims/sequencegenerator.h>
#include <ims/utils/distribution.h>
#include <ims/distributedalphabet.h>

namespace ims {

/** A class implementing random a sequence generator.
 * This class implements a random sequence generator, using character propabilities
 * form DistributedAlphabet.
 */
template <typename DistributedAlphabetType >
class RandomSequenceGenerator : public SequenceGenerator{

private:
	std::unique_ptr<Distribution> dist;
	DistributedAlphabetType alphabet;

public:
	RandomSequenceGenerator(DistributedAlphabetType alphabet);
	std::string getSequence(size_t length);
};


template <typename DistributedAlphabetType>
RandomSequenceGenerator<DistributedAlphabetType>::RandomSequenceGenerator(DistributedAlphabetType alphabet): alphabet(alphabet){
	std::vector<double> vec(alphabet.size());

	for(size_t i=0; i<alphabet.size(); i++){
		vec[i] = alphabet.getProbability(alphabet.getName(i));
	}
	dist = std::unique_ptr<Distribution>(new Distribution(vec));
}


template <typename DistributedAlphabetType>
std::string RandomSequenceGenerator<DistributedAlphabetType>::getSequence(size_t length){
	std::string sequence;

	for(size_t n=0; n<length; n++){
		sequence += alphabet.getName(dist->getRand()) ;
	}
	return sequence;
}

}
#endif

