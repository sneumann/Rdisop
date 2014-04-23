#include <stdlib.h>
#include <ims/utils/distribution.h>
#include <algorithm>

namespace ims {

Distribution::Distribution(std::vector<double> dist): p(dist), size(p.size()) {
	j.resize(size, 0);
	std::vector<size_t> smaller;
	std::vector<size_t> greater;

	for(size_t n=0; n< p.size(); n++){
		p[n] *= size;
		if(p[n] < 1){
			smaller.push_back(n);
		}else{
			greater.push_back(n);
		}
	}
	while( smaller.size() > 0 && greater.size() > 0){
		size_t k = greater.back();
		size_t l = smaller.back();

		j[l] = k;
		p[k] -=(1-p[l]);
		smaller.pop_back();
		if(p[k] < 1){
			greater.pop_back();
			smaller.push_back(k);
		}
	}
}

/**
* Get a random number.
*/
size_t Distribution::getRand(){
	size_t i = (size_t)( rand()/(RAND_MAX+1.0)*size );
	double r = (double(rand())/RAND_MAX);

	if(p[i] < r){
		return j[i];
	}
	return i;
}

}
