#include <cstring>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/base/exception/exception.h>
#include <ims/base/exception/ioexception.h>


class ExceptionTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ExceptionTest );
	CPPUNIT_TEST( testConstructor );
	CPPUNIT_TEST( testMessage );
	CPPUNIT_TEST( testIOException );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testConstructor();
	void testMessage();
	void testIOException();
};


CPPUNIT_TEST_SUITE_REGISTRATION( ExceptionTest );


void ExceptionTest::setUp() { }


void ExceptionTest::tearDown() { }


void ExceptionTest::testConstructor() {
	std::string mymessage = "the message";
	ims::Exception exception(mymessage);
	CPPUNIT_ASSERT_EQUAL(0, exception.message().compare(mymessage));

	ims::IOException ioexception(mymessage);
	CPPUNIT_ASSERT_EQUAL(0, ioexception.message().compare(mymessage));
}


void ExceptionTest::testMessage() {
	ims::Exception exception;
	exception.setMessage("the message");
	CPPUNIT_ASSERT_EQUAL(0, exception.message().compare("the message"));
}


void ExceptionTest::testIOException() {
	std::string mymessage("test: file not found");
	std::string resultmessage("");
	try {
		throw ims::IOException(mymessage);
	} catch (ims::Exception err) {
		resultmessage = err.message();
	}
	CPPUNIT_ASSERT_EQUAL(0, resultmessage.compare(mymessage));
}

