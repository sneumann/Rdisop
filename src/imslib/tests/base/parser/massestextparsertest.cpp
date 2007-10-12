#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/base/parser/massestextparser.h>
#include <ims/base/exception/ioexception.h>

using namespace ims;

class MassesTextParserTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(MassesTextParserTest);
		CPPUNIT_TEST(testParse);
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp();
		void tearDown();
		void testParse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(MassesTextParserTest);

void MassesTextParserTest::setUp() {
	std::ofstream ofs_masses_double_1("masses_double_1.test");
	ofs_masses_double_1 << " \t # \n \n \n  \t 0.1  0.2  \t  0.3 \n"
		"0.4 \n \t    ;\n  #     \n   ;;; \n 0.5";
	ofs_masses_double_1.close();
	std::ofstream ofs_masses_int_1("masses_int_1.test");
	ofs_masses_int_1 << "1  ;2\n  \t # 3 4 5 \n  \t  6;;;  \n \n \t";
	ofs_masses_int_1.close();	
}

void MassesTextParserTest::tearDown() {
	std::remove("masses_double_1.test");
	std::remove("masses_int_1.test");
}

void MassesTextParserTest::testParse() {
	MassesTextParser parser;
	parser.load("masses_double_1.test");
	MassesTextParser::ContainerType masses = parser.getElements();
	CPPUNIT_ASSERT(masses.size() == 5);
	CPPUNIT_ASSERT(masses.at(0) == 0.1);
	CPPUNIT_ASSERT(masses.at(1) == 0.2);
	CPPUNIT_ASSERT(masses.at(2) == 0.3);
	CPPUNIT_ASSERT(masses.at(3) == 0.4);
	CPPUNIT_ASSERT(masses.at(4) == 0.5);
	
	parser.load("masses_int_1.test");
	masses = parser.getElements();
	CPPUNIT_ASSERT(masses.size() == 3);
	CPPUNIT_ASSERT(masses.at(0) == 1.0);
	CPPUNIT_ASSERT(masses.at(1) == 2.0);
	CPPUNIT_ASSERT(masses.at(2) == 6.0);
}
