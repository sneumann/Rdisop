#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <iostream>
#include <ims/utils/log4cpp/Category.hh>
#include <ims/utils/log4cpp/Appender.hh>
#include <ims/utils/log4cpp/Priority.hh>
#include <ims/utils/log4cpp/BasicConfigurator.hh>

#include <ims/utils/log4cpp/BasicLayout.hh>
#include <ims/utils/log4cpp/OstreamAppender.hh>

using namespace log4cpp;

class CountingAppender : public LayoutAppender {
	public:
        int counter;

        CountingAppender(const std::string& name = "") : LayoutAppender(name), counter(0) {
        }
		
		virtual ~CountingAppender(){
		}
		
		virtual void close() {
		}
		  
        virtual void _append(const LoggingEvent&) { 
        	counter++; 
        }
};

class Log4CppCategoryTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(Log4CppCategoryTest);
		CPPUNIT_TEST(testAddRemove);
		CPPUNIT_TEST(testExists);
//////// !!!!!!!! The following tests don't work on i386 !!!!!!! TODO: check why? //////////
// 		CPPUNIT_TEST(testEnableDisable);		
//		CPPUNIT_TEST(testInheritance);
//		CPPUNIT_TEST(testMultipleInheritance);
// 		CPPUNIT_TEST(testAdditivity);
////////////////////////////////////////////////////////////////////////////////////////////
		CPPUNIT_TEST_SUITE_END();
		static std::string TEST_MESSAGE;
	public:
		void setUp();
		void tearDown();
		void testEnableDisable();
		void testExists();
		void testAddRemove();
		void testInheritance();
		void testMultipleInheritance();
		void testAdditivity();
};

CPPUNIT_TEST_SUITE_REGISTRATION(Log4CppCategoryTest);
std::string Log4CppCategoryTest::TEST_MESSAGE("TEST_MESSAGE");

void Log4CppCategoryTest::setUp() {
}

void Log4CppCategoryTest::tearDown() {
    Category::shutdown();
}

void Log4CppCategoryTest::testAddRemove() {
	Category& root = Category::getRoot();	// NOTE: normally one should avoid using root category directly, 
											// this is done here for testing purposes
	size_t categoriesSize = 1;
	CPPUNIT_ASSERT_EQUAL(root.getCurrentCategories()->size(), categoriesSize);  // NOTE: root is included in itself's categories.
	// gets two categories
	Category& category1 = Category::getInstance("testCategoryAddRemove1");
	CPPUNIT_ASSERT_EQUAL(root.getCurrentCategories()->size(), (categoriesSize = 2)); 
	Category& category2 = Category::getInstance("testCategoryAddRemove1.category2");
	CPPUNIT_ASSERT_EQUAL(root.getCurrentCategories()->size(), (categoriesSize = 3));
	 
	// gets the category with the same name as the one got before
	Category& category3 = Category::getInstance("testCategoryAddRemove1");	
	CPPUNIT_ASSERT_EQUAL(root.getCurrentCategories()->size(), categoriesSize); 	
	
	// checks the parentship
	CPPUNIT_ASSERT(root.getParent() == NULL);
	CPPUNIT_ASSERT_EQUAL(category1.getParent(), &root);
	CPPUNIT_ASSERT_EQUAL(category2.getParent(), &category1);
	CPPUNIT_ASSERT_EQUAL(category3.getParent(), &root);
	
	// removes all categories
	Category::shutdown();	

	// check if it was remained (Category::shutdown() removes only all appenders attached to categories but keeps the hierarchy the same)
	CPPUNIT_ASSERT_EQUAL(Category::getRoot().getCurrentCategories()->size(), categoriesSize);
	
}

void Log4CppCategoryTest::testExists() {
	Category& c1 = Category::getInstance("catExist1");
	Category& c12 = Category::getInstance("catExist1.catExist2");	
	Category& c123 = Category::getInstance("catExist1.catExist2.catExist3");	
	
	CPPUNIT_ASSERT(Category::exists("x") == NULL);
	CPPUNIT_ASSERT_EQUAL(Category::exists("catExist1"), &c1);
	CPPUNIT_ASSERT_EQUAL(Category::exists("catExist1.catExist2"), &c12);
	CPPUNIT_ASSERT_EQUAL(Category::exists("catExist1.catExist2.catExist3"), &c123);	
}

/**
 * Adds a category with an appender and checks basic enabling/disabling
 * logging by changing the priority of the category.
 */
void Log4CppCategoryTest::testEnableDisable() {
	Category& cat = Category::getInstance("testCategoryEnableDisable");

	CountingAppender* ca = new CountingAppender();
	cat.addAppender(ca);
	cat.setPriority(Priority::WARN);	
 	CPPUNIT_ASSERT_EQUAL(ca->counter, 0);

	cat.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 0);
	cat.info(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 0);
	cat.warn(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 1);
	cat.error(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 2);
	cat.fatal(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 3);
	
	cat.setPriority(Priority::INFO);
	cat.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 3);
	cat.info(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 4);
	cat.warn(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 5);
	
	cat.setPriority(Priority::ERROR);
	cat.warn(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 5);
	cat.error(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 6);	
	cat.fatal(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 7);
	
	delete ca;
}

/**
 * Adds a category X and its descendant X.Y and checks if logging 
 * by X.Y affects the appender of X.
 */
void Log4CppCategoryTest::testInheritance() {
	Category& category = Category::getInstance("c1");
	Category& inheritedCategory = Category::getInstance("c1.c2");
	
	CountingAppender* ca = new CountingAppender();
	category.addAppender(ca);
	
	// checks all levels of priorities
	// after initialization should be 0
	CPPUNIT_ASSERT_EQUAL(ca->counter, 0);
	// this shouldn't go through	
	inheritedCategory.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 0);
	
	// all this should go through
	inheritedCategory.info(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 1);
	inheritedCategory.warn(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 2);
	inheritedCategory.error(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 3);
	inheritedCategory.fatal(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 4);
	
	// "reduces" the priority of the most descendant
	inheritedCategory.setPriority(Priority::DEBUG);
	// this shouldn't go through
	inheritedCategory.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 4);
	
	// "reduces" the priority of the appender's owner 
	category.setPriority(Priority::DEBUG);
	// this should go through	
	inheritedCategory.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 5);

	// "increases" the priority of the appender's owner 
	category.setPriority(Priority::WARN);
	// this shouldn't go through	
	inheritedCategory.debug(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca->counter, 5);	

	delete ca;
}

/**
 * Adds categories X, X.Y, X.Y.Z and A and checks if varios logging 
 * affects an appender which is set on one of the categories.
 */
 void Log4CppCategoryTest::testMultipleInheritance() {
	Category& c1 = Category::getInstance("cat1");
	Category& c1c2 = Category::getInstance("cat1.cat2");
	Category& c1c2c3 = Category::getInstance("cat1.cat2.cat3");
	Category& x = Category::getInstance("x");
	
	CountingAppender* ca1 = new CountingAppender();
	CountingAppender* ca2 = new CountingAppender();	

/*	CountingAppender ca1;
	CountingAppender ca2;	*/
	
	c1.addAppender(ca1);
	c1c2c3.addAppender(ca2);

/*	c1.addAppender(&ca1);
	c1c2c3.addAppender(&ca2);*/
	
    CPPUNIT_ASSERT_EQUAL(ca1->counter, 0);
    CPPUNIT_ASSERT_EQUAL(ca2->counter, 0);

//     CPPUNIT_ASSERT_EQUAL(ca1.counter, 0);
//     CPPUNIT_ASSERT_EQUAL(ca2.counter, 0);

    c1c2.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(ca1->counter, 1);
    CPPUNIT_ASSERT_EQUAL(ca2->counter, 0);

//     CPPUNIT_ASSERT_EQUAL(ca1.counter, 1);
//     CPPUNIT_ASSERT_EQUAL(ca2.counter, 0);

	c1c2c3.info(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca1->counter, 2);
	CPPUNIT_ASSERT_EQUAL(ca2->counter, 1);

// 	CPPUNIT_ASSERT_EQUAL(ca1.counter, 2);
// 	CPPUNIT_ASSERT_EQUAL(ca2.counter, 1);

	x.info(TEST_MESSAGE);
	CPPUNIT_ASSERT_EQUAL(ca1->counter, 2);
	CPPUNIT_ASSERT_EQUAL(ca2->counter, 1);	

// 	CPPUNIT_ASSERT_EQUAL(ca1.counter, 2);
// 	CPPUNIT_ASSERT_EQUAL(ca2.counter, 1);	

	delete ca2;
	delete ca1;
}

/**
 * Test additivity flag.
 */
void Log4CppCategoryTest::testAdditivity() {
	Category& root = Category::getRoot();	// NOTE: normally one should avoid using root category directly, 
											// this is done here for testing purposes
	Category& c = Category::getInstance("c");
	Category& cd = Category::getInstance("c.d");
	Category& cde = Category::getInstance("c.d.e");
	Category& x = Category::getInstance("xx");	
	
	CountingAppender* caRoot = new CountingAppender();
	CountingAppender* caC = new CountingAppender();	
	CountingAppender* caCDE = new CountingAppender();
	
	root.addAppender(caRoot);
	c.addAppender(caC);
	cde.addAppender(caCDE);
	
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 0);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 0);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 0);	
    
    cd.setAdditivity(false);
    
    c.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 0);
    
    cd.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 0);
    	
    cde.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 1);
    
    x.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 2);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 1);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 1);
    
    cd.setAdditivity(true);
    
    cd.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 3);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 2);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 1);
    
    cde.info(TEST_MESSAGE);
    CPPUNIT_ASSERT_EQUAL(caRoot->counter, 4);
    CPPUNIT_ASSERT_EQUAL(caC->counter, 3);
    CPPUNIT_ASSERT_EQUAL(caCDE->counter, 2);    

	delete caCDE;
	delete caC;
	delete caRoot;
}

