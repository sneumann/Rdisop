#ifndef IMS_DISTRIBUTION_H
#define IMS_DISTRIBUTION_H

#include <unistd.h>

#include <vector>

namespace ims {

/** A class implementing a discrete distribution.
 * This class implements a discrete distribution, using the alias method.
 */
class Distribution {
	public:
		Distribution(std::vector<double> dist);
		size_t getRand();

	private:
		std::vector<double> p;
		std::vector<size_t> j;
		size_t size;
};

}

#endif
