#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <fstream>
#include <utility>

#include <ims/calib/linepairstabber.h>

using namespace std;

class LinePairStabberTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LinePairStabberTest );
	CPPUNIT_TEST( testStabOrdinate );
	CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void testStabOrdinate();
	
	private:
		std::vector<std::pair<double,double> >* points;
};

CPPUNIT_TEST_SUITE_REGISTRATION( LinePairStabberTest );

//currently unused
#if 0
/**
 * Reads points from a file. The vector should be empty.
 */
void read_points(char* filename, std::vector<std::pair<double,double> >* points) {
	std::ifstream ifs(filename);
	std::string line;
	double coord_x, coord_y;
	while (std::getline(ifs, line)) {
		size_t i = line.find_first_not_of(" \t");
		if (i == std::string::npos || line[i] == '#')
			continue; // skip comment lines
		std::istringstream input(line);
		input >> coord_x >> coord_y;
		points->push_back(std::pair<double,double>(coord_x, coord_y));
	}
}
#endif

void LinePairStabberTest::setUp() {
	points = new std::vector<std::pair<double,double> >;
}


void LinePairStabberTest::tearDown() {
	delete points;
}


void LinePairStabberTest::testStabOrdinate() {
	points->push_back(make_pair(2.2, -8.0));
	points->push_back(make_pair(1.7, -10.0));
	points->push_back(make_pair(2.8, -7.5));
	
	pair<double,double> result = ims::LinePairStabber::stab_ordinate(*points, 1.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, result.first, 1e-5);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-13.4, result.second, 1e-5);
}


