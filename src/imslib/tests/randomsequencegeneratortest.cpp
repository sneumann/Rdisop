#include <cstring>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/randomsequencegenerator.h>

using namespace ims;

class RandomSequenceGeneratorTest : public CppUnit::TestFixture {
	
	typedef DistributedAlphabet alphabet_type;
	
	CPPUNIT_TEST_SUITE( RandomSequenceGeneratorTest );
	CPPUNIT_TEST( testGetSequence );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testGetSequence();
};


CPPUNIT_TEST_SUITE_REGISTRATION( RandomSequenceGeneratorTest );


void RandomSequenceGeneratorTest::setUp() { }


void RandomSequenceGeneratorTest::tearDown() { }


void RandomSequenceGeneratorTest::testGetSequence() {
	alphabet_type alphabet;

	alphabet.push_back("a", 1, 0.1);
	alphabet.push_back("a", 1, 0.2);
	alphabet.push_back("a", 1, 0.3);
	alphabet.push_back("a", 1, 0.4);

	ims::RandomSequenceGenerator< alphabet_type > generator(alphabet);
	
	std::string s = generator.getSequence(23);

	CPPUNIT_ASSERT_EQUAL((size_t)23, s.size());
}



