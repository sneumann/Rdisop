#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/decomp/twomassdecomposer.h>
#include <ims/decomp/twomassdecomposer2.h>
#include <ims/alphabet.h>

using namespace std;
using namespace ims;

class TwoMassDecomposer2Test : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(TwoMassDecomposer2Test);
		CPPUNIT_TEST(testGetAllDecompositions);
		CPPUNIT_TEST_SUITE_END();

	public:
		void testGetAllDecompositions();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TwoMassDecomposer2Test);

void TwoMassDecomposer2Test::testGetAllDecompositions() {
	/*
	TODO use a different order of weights
	TODO use an alphabet with gcd!=1 somewhere
	*/
	Alphabet a;
	a.push_back("", 197);
	a.push_back("", 218);
	a.push_back("", 323);
	a.push_back("", 460);

	Alphabet b;
	b.push_back("", 192);
	b.push_back("", 240);
	b.push_back("", 310);
	b.push_back("", 459);

	Weights weights_a(a.getMasses(), 1);
	Weights weights_b(b.getMasses(), 1);

	TwoMassDecomposer2<> decomposer(weights_a, weights_b);
	TwoMassDecomposer<> decomposer_old(weights_a, weights_b);

	for (TwoMassDecomposer2<>::value_type mass_a = 0; mass_a < 200000; mass_a += 100) {
		TwoMassDecomposer2<>::decompositions_type decomps = decomposer.getAllDecompositions(mass_a, mass_a+50);
		CPPUNIT_ASSERT_EQUAL(decomposer_old.getAllDecompositions(mass_a, mass_a+50).size(), decomps.size());
		TwoMassDecomposer2<>::decompositions_type::const_iterator cit = decomps.begin();
		for ( ; cit != decomps.end(); ++cit) {
			TwoMassDecomposer2<>::value_type total_a = 0;
			TwoMassDecomposer2<>::value_type total_b = 0;

			assert(cit->size() == weights_a.size());
			assert(cit->size() == weights_b.size());

			for (size_t i = 0; i < cit->size(); ++i) {
				total_a += (*cit)[i] * weights_a[i];
				total_b += (*cit)[i] * weights_b[i];
			}
			CPPUNIT_ASSERT_EQUAL(mass_a, total_a);
			CPPUNIT_ASSERT_EQUAL(mass_a+50, total_b);
		}
	}
}
