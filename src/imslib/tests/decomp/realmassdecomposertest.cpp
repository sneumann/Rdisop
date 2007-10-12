/**
 * realmassdecomposertest.cpp
 *
 * Author: Anton Pervukhin Anton.Pervukhin@cebitec.uni-bielefeld.de
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <ims/utils/math.h>

#include <ims/decomp/realmassdecomposer.h>
#include <ims/decomp/decomputils.h>


using namespace std;
using namespace ims;

class RealMassDecomposerTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(RealMassDecomposerTest);
		CPPUNIT_TEST(testGetDecompositions);
		CPPUNIT_TEST_SUITE_END();
	private:
		typedef RealMassDecomposer decomposer_type;
		typedef decomposer_type::decompositions_type decompositions_type;

	public:
		void testGetDecompositions();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RealMassDecomposerTest);

void RealMassDecomposerTest::testGetDecompositions() {
	typedef Weights::alphabet_mass_type alphabet_mass_type;
	typedef Weights::alphabet_masses_type alphabet_masses_type;

	alphabet_mass_type mono_mass_H = 1.007825, mono_mass_C = 12.0,
					   mono_mass_N = 14.003074, mono_mass_O = 15.994915,
					   mono_mass_P = 30.973762, mono_mass_S = 31.972071;

	alphabet_masses_type mono_masses;
	mono_masses.push_back(mono_mass_H);
	mono_masses.push_back(mono_mass_C);
	mono_masses.push_back(mono_mass_N);
	mono_masses.push_back(mono_mass_O);
	mono_masses.push_back(mono_mass_P);
	mono_masses.push_back(mono_mass_S);

	Weights alphabet_weights(mono_masses, 0.00001);

	decomposer_type decomposer(alphabet_weights);

	double step = 100.0, firstMass = 100.0, lastMass = 1500.0, error = 0.0001;
	for (double mass = firstMass; mass < lastMass; mass += step) {
		decompositions_type decompositions =
				decomposer.getDecompositions(mass, error);
		for (decompositions_type::iterator pos = decompositions.begin();
			 						pos != decompositions.end(); ++pos) {
			double parent_mass =
				DecompUtils::getParentMass(alphabet_weights, *pos);
			CPPUNIT_ASSERT(fabs(parent_mass - mass) <= error);
		}
	}
}
