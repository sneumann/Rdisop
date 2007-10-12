#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Executable point of all test source files. Implementation based on the following sources:
 * http://cppunit.sourceforge.net/doc/lastest/money_example.html
 * http://cppunit.sourceforge.net/doc/lastest/cppunit_cookbook.html.
 */
int main(int argc, char* argv[]) {
	(void)argc; // avoids "unused parameter" warning
	(void)argv;
	
	// Get the top level suite from the registry
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	
	// Adds the test to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	//CppUnit::QtUi::TestRunner runner;
	runner.addTest( suite );
	
	// Change the default outputter to a compiler error format outputter
	runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );
	
	// Run the tests.
	bool wasSucessful = runner.run();
	//runner.run();
	
	// Return error code 1 if one of the tests failed.
	return wasSucessful ? 0 : 1;
}
