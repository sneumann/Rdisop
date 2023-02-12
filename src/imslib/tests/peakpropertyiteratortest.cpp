#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <ims/masspeak.h>
#include <ims/peaklist.h>
#include <ims/peakpropertyiterator.h>

using namespace ims;

class PeakPropertyIteratorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE( PeakPropertyIteratorTest );
	CPPUNIT_TEST( testInputOutput );
	CPPUNIT_TEST( testBidirectionality );
	CPPUNIT_TEST( testRandomAccess );
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp() {}
	void tearDown() {}
	/** Test basic input and output properties, to be defined for a forward iterator. */
	void testInputOutput();
	/** Test additional properties of a bidirectonal iterator (namely the --operator). */
	void testBidirectionality();
	/** Test additional properties of a random access iterator (namely +,-,+=,-=,<,>,<=,>=,[]). */
	void testRandomAccess();
private:
	typedef MassPeak<int> peak_type;
	typedef PeakList<peak_type> peaklist_type;
	/** Factory for simple peaklists with masses from 1 to 10. */
	std::unique_ptr<peaklist_type> createPeaklist();
};

CPPUNIT_TEST_SUITE_REGISTRATION( PeakPropertyIteratorTest );

std::unique_ptr<PeakPropertyIteratorTest::peaklist_type> PeakPropertyIteratorTest::createPeaklist() {
	std::unique_ptr<peaklist_type> p(new peaklist_type);
	for (int i=1; i<=10; ++i) {
		p->push_back(i);
	}
	return p;
}

void PeakPropertyIteratorTest::testInputOutput() {
	std::unique_ptr<peaklist_type> pl=createPeaklist();
	peaklist_type::property_iterator<peak_type::MassGetter>::type it;
	
	int i=1;
	for (it=pl->begin<peak_type::MassGetter>(); it!=pl->end<peak_type::MassGetter>(); ++it,++i) {
		// test * operator
		CPPUNIT_ASSERT_EQUAL(i, *it);
		// test ==,!= operator
		peaklist_type::property_iterator<peak_type::MassGetter>::type it2=it;
		CPPUNIT_ASSERT(it == it2);
		++it2;
		CPPUNIT_ASSERT(it != it2);
		// write access
		*it=-i;
	}

	// test if write operations were successful
	i=-1;
	for (it=pl->begin<peak_type::MassGetter>(); it!=pl->end<peak_type::MassGetter>(); ++it,--i) {
		CPPUNIT_ASSERT_EQUAL(i, *it);
	}
	
	// test operator->
	typedef MassPeak<std::pair<int,int> > pair_peak_type;
	typedef PeakList<pair_peak_type> pair_peaklist_type;
	pair_peaklist_type pair_pl;
	pair_pl.push_back(std::pair<int,int>(1,2));
	pair_pl.push_back(std::pair<int,int>(3,4));
	pair_peaklist_type::property_iterator<pair_peak_type::MassGetter>::type pair_it;
	pair_it = pair_pl.begin<pair_peak_type::MassGetter>();
	CPPUNIT_ASSERT_EQUAL(1, pair_it->first);
	CPPUNIT_ASSERT_EQUAL(2, pair_it->second);
	++pair_it;
	CPPUNIT_ASSERT_EQUAL(3, pair_it->first);
	CPPUNIT_ASSERT_EQUAL(4, pair_it->second);
	pair_it->second=77;
	CPPUNIT_ASSERT_EQUAL(77, pair_it->second);
}

void PeakPropertyIteratorTest::testBidirectionality() {
	std::unique_ptr<peaklist_type> pl=createPeaklist();
	peaklist_type::property_iterator<peak_type::MassGetter>::type it,it2;
	
	int i=10;
	it=pl->end<peak_type::MassGetter>();
	for (--it; it!=pl->begin<peak_type::MassGetter>(); --it,--i) {
		CPPUNIT_ASSERT_EQUAL(i, *it);
		// test traversal of prefix operators
		++it;
		--it;
		CPPUNIT_ASSERT_EQUAL(i, *it);
		// test traversal of postfix operators
		it++;
		it--;
		CPPUNIT_ASSERT_EQUAL(i, *it);
	}

	// Test prefix/postfix properties
	it=pl->begin<peak_type::MassGetter>();
	it2=it++;
	CPPUNIT_ASSERT_EQUAL(1, *it2);
	CPPUNIT_ASSERT_EQUAL(2, *it);
	it2=it--;
	CPPUNIT_ASSERT_EQUAL(2, *it2);
	CPPUNIT_ASSERT_EQUAL(1, *it);
	it2=++it;
	CPPUNIT_ASSERT_EQUAL(2, *it2);
	CPPUNIT_ASSERT_EQUAL(2, *it);
	it2=--it;
	CPPUNIT_ASSERT_EQUAL(1, *it2);
	CPPUNIT_ASSERT_EQUAL(1, *it);
}

void PeakPropertyIteratorTest::testRandomAccess() {
	std::unique_ptr<peaklist_type> pl=createPeaklist();
	peaklist_type::property_iterator<peak_type::MassGetter>::type it,it2;
	
	it=pl->begin<peak_type::MassGetter>();
	// test +=
	it+=5;
	CPPUNIT_ASSERT_EQUAL(6, *it);
	// test []
	CPPUNIT_ASSERT_EQUAL(6, it[0]);
	CPPUNIT_ASSERT_EQUAL(8, it[2]);
	CPPUNIT_ASSERT_EQUAL(4, it[-2]);
	// test it + n
	CPPUNIT_ASSERT_EQUAL(9, *(it+3));
	// test n + it
	CPPUNIT_ASSERT_EQUAL(9, *(3+it));
	// test -
	CPPUNIT_ASSERT_EQUAL(3, *(it-3));
	// test it - it
	it2=pl->begin<peak_type::MassGetter>();
	CPPUNIT_ASSERT_EQUAL((ptrdiff_t)5, it-it2);
	CPPUNIT_ASSERT_EQUAL((ptrdiff_t)-5, it2-it);
	// test -=
	it-=5;
	CPPUNIT_ASSERT_EQUAL(1, *it);
	// test <, >, <=, <=
	it=pl->begin<peak_type::MassGetter>();
	it2=it+4;
	for (;it!=pl->end<peak_type::MassGetter>();++it) {
		if (*it==5) {
			CPPUNIT_ASSERT(it<=it2);
			CPPUNIT_ASSERT(it>=it2);
			CPPUNIT_ASSERT(!(it<it2));
			CPPUNIT_ASSERT(!(it>it2));
		}
		if (*it<5) {
			CPPUNIT_ASSERT(it<=it2);
			CPPUNIT_ASSERT(!(it>=it2));
			CPPUNIT_ASSERT(it<it2);
			CPPUNIT_ASSERT(!(it>it2));
		}
		if (*it>5) {
			CPPUNIT_ASSERT(!(it<=it2));
			CPPUNIT_ASSERT(it>=it2);
			CPPUNIT_ASSERT(!(it<it2));
			CPPUNIT_ASSERT(it>it2);
		}
	}
}
