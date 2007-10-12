#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

#include "config.h"

#include <ims/massintensitypeak.h>
#include <ims/peaklist.h>

using namespace std;
using namespace ims;

typedef MassIntensityPeak<double, double> peak_t;
typedef PeakList<peak_t> peaklist_t;

int main()
{
	peak_t p1(1.0, 2.0), p2(1.1,2.1);
	peaklist_t pl;
	
	pl.push_back(p1);
	pl.push_back(p2);
	
	peaklist_t::property_iterator<peak_t::MassGetter>::type it = pl.begin<peak_t::MassGetter>();
	for ( ; it != pl.end<peak_t::MassGetter>(); ++it) {
        cout << *it << endl;
	}

	cout << "---" << endl;

	peaklist_t::property_iterator<peak_t::IntensityGetter>::type it2 = pl.begin<peak_t::IntensityGetter>();
	for ( ; it2!=pl.end<peak_t::IntensityGetter>(); ++it2) {
		cout << *it2 << endl;
	}
}
