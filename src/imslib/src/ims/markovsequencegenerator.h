#ifndef IMS_MARKOVSEQUENCEGENERATOR_H
#define IMS_MARKOVSEQUENCEGENERATOR_H

#include <memory>

#include <ims/sequencegenerator.h>
#include <ims/utils/distribution.h>
#include <ims/distributedalphabet.h>
#include <assert.h>

namespace ims {

/**
 * A class implementing a markov sequence generator.
 * This class implements a sequence generator, using a markov model.
 * Initial character propabilities are taken form DistributedAlphabet.
 * Transition propabilities are given as extra parameter.
 */
template <typename DistributedAlphabetType>
class MarkovSequenceGenerator : public SequenceGenerator {

private:
	std::vector<Distribution> dists;
	std::unique_ptr<Distribution> start_dist;
	DistributedAlphabetType alphabet;

public:
	MarkovSequenceGenerator(DistributedAlphabetType alphabet, std::vector<std::vector<double> >& matrix);
	std::string getSequence(size_t length);
};



template <typename DistributedAlphabetType>
MarkovSequenceGenerator<DistributedAlphabetType>::
		MarkovSequenceGenerator(DistributedAlphabetType alphabet,
		std::vector<std::vector<double> >& matrix): alphabet(alphabet) {


	/** Initial character propabilities **/
	std::vector<double> start_p(alphabet.size());
	for(size_t i=0; i<alphabet.size(); i++){
		start_p[i] = alphabet.getProbability(alphabet.getName(i));
	}
	start_dist = std::unique_ptr<Distribution>(new Distribution(start_p));


	/** matrix **/
	for(size_t n=0; n<matrix.size(); n++){
		dists.push_back(Distribution(matrix.at(n)));
	}

}


template <typename DistributedAlphabetType>
std::string MarkovSequenceGenerator<DistributedAlphabetType>::getSequence(size_t length) {
	std::string sequence;

	if(length == 0){
		return sequence;
	}

	size_t current = start_dist->getRand();
	sequence = alphabet.getName(current);

	for(size_t n = 1; n < length; n++){
		current = dists[current].getRand();
		sequence += alphabet.getName(current);
	}
	return sequence;
}

}
#endif

