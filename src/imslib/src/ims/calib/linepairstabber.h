#ifndef IMS_LINEPAIRSTABBER_H
#define IMS_LINEPAIRSTABBER_H

#include <vector>

namespace ims {

// TODO: namespace or class?
namespace LinePairStabber {
	std::pair<double,double> stab_ordinate(
		const std::vector<std::pair<double,double> >& points,
		double epsilon
	);
}

}
#endif
