#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/utils/log4cpp/Category.hh>
#include <ims/utils/log4cpp/Appender.hh>
#include <ims/utils/log4cpp/OstreamAppender.hh>
#include <ims/utils/log4cpp/Priority.hh>
#include <ims/utils/log4cpp/BasicConfigurator.hh>
#include <ims/utils/log4cpp/BasicLayout.hh>
#include <ims/utils/log4cpp/PatternLayout.hh>

using namespace log4cpp;

/**
 * TODO: Add more tests for different types of LayoutAppender: FileAppender, OstreamAppender, StringQueueAppender.
 * Add tests for layouts: BasicLayout, SimpleLayout, PatternLayout. 
 * Add tests for configurators: BasicConfigurator, PropertyConfigurator.
 */
class Log4CppAppenderTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(Log4CppAppenderTest);
		CPPUNIT_TEST(testAppender1);
		CPPUNIT_TEST(testAppender2);
//		CPPUNIT_TEST(testPatternLayout);
//		CPPUNIT_TEST(testPropertyConfigurator);				
		CPPUNIT_TEST_SUITE_END();
		void showPattern(const std::string& pattern, PatternLayout* layout, Category& cat);		
//		log4cpp::Category root;		
	public:
		void testAppender1();
		void testAppender2();
		void testPatternLayout();
		void testPropertyConfigurator();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Log4CppAppenderTest);

void Log4CppAppenderTest::setUp() {
//	root = log4cpp::Category::getRoot();
//	root.removeAllAppenders();
}

void Log4CppAppenderTest::tearDown() {
    Category::shutdown();
}

/**
 * Adds an appender and sees if it can be retrieved.
 */
void Log4CppCategoryTest::testAppender1() {
	Category& category = Category::getInstance("testCategory1");
	Appender* appender1 = new OstreamAppender("testAppender1", &std::cout);
	category.addAppender(appender1);
	AppenderSet appenders = category.getAllAppenders();
	CPPUNIT_ASSERT(appenders.size() == 1);
	CPPUNIT_ASSERT_EQUAL(*(appenders.begin()), appender1);
}

/**
 * Adds an appender X, Y, removes X and checks if Y is the only remaining appender.
 */
void Log4CppCategoryTest::testAppender2() {
	
	Appender* appender1 = new OstreamAppender("testAppender1", &std::cout);
	Appender* appender2 = new OstreamAppender("testAppender2", &std::cout);	
	
	Category& category = Category::getInstance("testCategoryAppender2");
	category.addAppender(appender1);
	category.addAppender(appender2);
	category.removeAppender("testAppender1");
	
	AppenderSet appenders = category.getAllAppenders();
	CPPUNIT_ASSERT(appenders.size() == 1);
	CPPUNIT_ASSERT_EQUAL(*(appenders.begin()), appender2);
}

void Log4CppAppenderTest::testPatternLayout() {
    BasicConfigurator::configure();
    PatternLayout *layout = new PatternLayout();
    Category::getRoot().getAppender()->setLayout(layout);

    Category& cat = Category::getInstance("cat1");
    
    showPattern("%% %r %c:%d (%R / %r) [%p] %m %% %n", layout, cat);

    // category test
    showPattern(">%c{2}<%n", layout, Category::getInstance("c1.c2.c3.c4"));

    // test date format
    showPattern("%d{%d %b %Y %H:%M:%S.%l} %m %n", layout, cat);
    showPattern("%d{%d %b %Y %H:%M:%S.%l", layout, cat);
    showPattern("%d%n", layout, cat);

    showPattern("%m %d%n", layout, cat);
    showPattern("%m %d{%H:%M:%S.%l %d %b %Y}%n", layout, cat);
}

void Log4CppAppenderTest::testPropertyConfigurator(){
	
}

void Log4CppAppenderTest::showPattern(const std::string& pattern, PatternLayout* layout, Category& cat) {
    try {	
        layout->setConversionPattern(pattern);
        cat.error("message");
    } catch(ConfigureFailure& f) {
        std::cerr << "configure failure: " << f.what() << std::endl;
    }	
}
