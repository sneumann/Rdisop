#include <cstring>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/markovsequencegenerator.h>

using namespace ims;

class MarkovSequenceGeneratorTest : public CppUnit::TestFixture {
	
	typedef DistributedAlphabet alphabet_type;
	
	CPPUNIT_TEST_SUITE( MarkovSequenceGeneratorTest );
	CPPUNIT_TEST( testGetSequence );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testGetSequence();
};


CPPUNIT_TEST_SUITE_REGISTRATION( MarkovSequenceGeneratorTest );


void MarkovSequenceGeneratorTest::setUp() { }


void MarkovSequenceGeneratorTest::tearDown() { }


void MarkovSequenceGeneratorTest::testGetSequence() {
	alphabet_type alphabet;

	alphabet.push_back("a", 23, 1);
	alphabet.push_back("b", 23, 1);
	alphabet.push_back("c", 23, 1);
	alphabet.push_back("d", 23, 1);
	alphabet.normalizeDistribution();

	std::vector< std::vector<double> > matrix;
	for(size_t i=0; i<alphabet.size(); i++){
		std::vector<double> p;
		for(size_t n=0; n<alphabet.size(); n++){
			p.push_back((double(rand())/RAND_MAX));
		}
		
		double sum=0;
		for(size_t n=0; n<p.size(); n++){
			sum += p[n];
		}
		for(size_t n=0; n<p.size(); n++){
			p[n] = p[n]/sum;
		}
		matrix.push_back(p);
	}

	ims::MarkovSequenceGenerator< alphabet_type > msg(alphabet, matrix);
	
	std::string s = msg.getSequence(23);

	CPPUNIT_ASSERT_EQUAL((size_t)23, s.size());
}
