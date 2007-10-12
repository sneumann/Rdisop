/**
 * integermassdecomposertest.cpp
 * 
 * Author: Anton Pervukhin Anton.Pervukhin@cebitec.uni-bielefeld.de
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <vector>
#include <algorithm>
#include <iostream>
#include <ims/decomp/integermassdecomposer.h>
#include <ims/weights.h>

using namespace ims;

template <typename DecomposerType>
class IntegerMassDecomposerTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(IntegerMassDecomposerTest);
		CPPUNIT_TEST(testConstructor);
		CPPUNIT_TEST(testExist);
		CPPUNIT_TEST(testGetDecomposition);
		CPPUNIT_TEST(testGetNumberOfDecompositions);
		CPPUNIT_TEST(testGetAllDecompositions);
		CPPUNIT_TEST_SUITE_END();
	private:
		typedef DecomposerType decomposer_type;
		typedef typename decomposer_type::value_type value_type;
		typedef typename decomposer_type::decomposition_value_type 
											decomposition_value_type;
		typedef typename decomposer_type::decomposition_type 
											decomposition_type;
		typedef typename decomposer_type::decompositions_type 
											decompositions_type;
		typedef typename Weights::alphabet_mass_type alphabet_mass_type;
		typedef typename Weights::alphabet_masses_type 
											alphabet_masses_type;
		typedef typename Weights::weights_type weights_type;
		
		void checkDecomposition(const decomposition_value_type* elements,
			const decompositions_type& decompositions);
	
		void checkDecomposition(decomposer_type *decomposer, value_type mass);
		
		Weights* weights;
	public:
		void setUp();
		void tearDown();
		void testConstructor();
		void testExist();
		void testGetDecomposition();
		void testGetNumberOfDecompositions();
		void testGetAllDecompositions();		
};

typedef IntegerMassDecomposerTest<IntegerMassDecomposer<> > 	DecomposerType;
typedef IntegerMassDecomposerTest<IntegerMassDecomposer<long unsigned int, 
				long unsigned int> > DecomposerLongDecompositionValueType;
CPPUNIT_TEST_SUITE_REGISTRATION(DecomposerType);
CPPUNIT_TEST_SUITE_REGISTRATION(DecomposerLongDecompositionValueType);

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::setUp() {

	alphabet_mass_type a0 = 0.6, a1 = 0.7, a2 = 1.1, a3 = 1.5;
	
	alphabet_masses_type masses;		
	masses.push_back(a0);
	masses.push_back(a1);
	masses.push_back(a2);
	masses.push_back(a3);
	
	weights = new Weights(masses, 0.1);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::tearDown() {
	delete weights;
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::testConstructor() {
	decomposer_type *decomposer = new decomposer_type(*weights);
	CPPUNIT_ASSERT(decomposer->exist(0) == true);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::testExist() {

	decomposer_type *decomposer = new decomposer_type(*weights);	

	CPPUNIT_ASSERT(decomposer->exist(0) == true);

	CPPUNIT_ASSERT(!decomposer->exist(static_cast<value_type>( 
				(weights->getAlphabetMass(0)-0.1) / weights->getPrecision())));

	for (weights_type::size_type i = 0; i < weights->size(); ++i) {
		CPPUNIT_ASSERT(decomposer->exist(weights->getWeight(i)) == true);
		CPPUNIT_ASSERT(decomposer->exist(weights->getWeight(i) * 2) == true);	
	}

	CPPUNIT_ASSERT(decomposer->exist(14) == true);
	CPPUNIT_ASSERT(decomposer->exist(16) == false);
	CPPUNIT_ASSERT(decomposer->exist(13) == true);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::testGetNumberOfDecompositions() {
	decomposer_type *decomposer = new decomposer_type(*weights);		
	CPPUNIT_ASSERT(decomposer->getNumberOfDecompositions(0) == 1);
	CPPUNIT_ASSERT(decomposer->getNumberOfDecompositions(
		static_cast<value_type>((weights->getAlphabetMass(0)-0.1) / 
								weights->getPrecision()) ) == 0);
	CPPUNIT_ASSERT(decomposer->getNumberOfDecompositions(weights->getWeight(0))  == 1);
	for (weights_type::size_type i=0; i < weights->size(); ++i) {
		CPPUNIT_ASSERT(decomposer->getNumberOfDecompositions(weights->getWeight(i)) > 0);
	}
	CPPUNIT_ASSERT(decomposer->getNumberOfDecompositions(100) == 40);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::testGetDecomposition() {
	decomposer_type *decomposer = new decomposer_type(*weights);

	checkDecomposition(decomposer, 39);
	checkDecomposition(decomposer, 34);

	for (weights_type::size_type i = 0; i < weights->size(); ++i) {
		decomposition_type decomposition = 
			decomposer->getDecomposition(weights->getWeight(i));
		CPPUNIT_ASSERT(decomposition.empty() == false);
	}
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::
checkDecomposition(decomposer_type *decomposer, value_type mass) {
	decomposition_type decomposition = decomposer->getDecomposition(mass);
	CPPUNIT_ASSERT(decomposition.size() == weights->size());
	alphabet_mass_type sum = 0;
	for (typename decomposition_type::const_iterator 
				it = decomposition.begin(); it < decomposition.end(); ++it) {
		sum += weights->getAlphabetMass(it-decomposition.begin()) * (*it);
	}
	CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, mass * weights->getPrecision(), 1.0e-5);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::testGetAllDecompositions() {
	decomposer_type *decomposer = new decomposer_type(*weights);	
	decompositions_type decompositions = decomposer->getAllDecompositions(44);
	CPPUNIT_ASSERT(decompositions.size() == 6);
	decomposition_value_type elements[] = {0, 2, 0, 2};
	checkDecomposition(elements, decompositions);
	decomposition_value_type elements2[] = {0, 1, 2, 1};
	checkDecomposition(elements2, decompositions);
	decomposition_value_type elements3[] = {3, 0, 1, 1};	
	checkDecomposition(elements3, decompositions);
	decomposition_value_type elements4[] = {0, 0, 4, 0};	
	checkDecomposition(elements4, decompositions);
	decomposition_value_type elements5[] = {2, 3, 1, 0};	
	checkDecomposition(elements5, decompositions);
	decomposition_value_type elements6[] = {5, 2, 0, 0};	
	checkDecomposition(elements6, decompositions);
}

template <typename DecomposerType>
void IntegerMassDecomposerTest<DecomposerType>::
checkDecomposition(const decomposition_value_type* elements, 
					const decompositions_type& decompositions) {

	decomposition_type decomposition;
	decomposition.reserve(weights->size());
	for (weights_type::size_type i = 0; i < weights->size(); ++i) {
		decomposition.push_back(elements[i]);
	}

	typename decompositions_type::const_iterator decompositionPos;
	decompositionPos = find(decompositions.begin(), decompositions.end(), 
							decomposition);
	CPPUNIT_ASSERT(decompositionPos != decompositions.end());
	
	typename decompositions_type::const_iterator decompositionPos2;
	if (decompositionPos != decompositions.end()) {
		decompositionPos2 = find(++decompositionPos, decompositions.end(), 
								decomposition);
	}
	CPPUNIT_ASSERT(decompositionPos2 == decompositions.end());		
}

