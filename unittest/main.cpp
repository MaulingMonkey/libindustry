// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - Many disabled tests (changing to using a directory structure once more for libindustry SVN on sourceforge)
// Jul 08, 2006 - Switched to using the Boost Unit Test Framework
// May 20, 2006 - Created

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4535 ) //calling _set_se_translator()
#pragma warning( disable : 4244 ) //possible initialization data loss conversion
#pragma warning( disable : 4267 ) //possible argument       data loss conversion
#endif

#include <industry/config.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>
#include <iostream>

using boost::unit_test::test_suite;

#if defined( _MSC_VER )
#pragma warning(pop)
#endif

typedef void (*test_function_ptr)();

void test_algorithm();
void test_container();
void test_factory();
//void test_image();
void test_inherit();
void test_iterator_cast();
void test_iterator_n();
void test_iterator_traits();
void test_iterator_virtual();
void test_math_fixed();
void test_math_vector();
void test_multitype();
void test_multi_iterator();
void test_range();
//void test_utility();
void test_range_numeric_iterator();
void test_template_crtp();
void test_template_group();
void test_template_tt();
void test_traits_processor();
void test_algorithm_call();
void test_algorithm_transform();
void test_algorithm_transform_memberptr();
void test_container_default_allocation_policy();
void test_languages_ruby();

#if defined( INDUSTRY_OS_WINDOWS )
void test_api_windows_registry();

#endif

test_suite * init_unit_test_suite( int , char *[] ) {
	using namespace std;
	cout << "Test started..." << endl;

	test_suite * test = BOOST_TEST_SUITE( "libindustry master test suite" );

	test->add(BOOST_TEST_CASE(test_languages_ruby));
	test->add(BOOST_TEST_CASE(test_container));
	test->add(BOOST_TEST_CASE(test_container_default_allocation_policy));
	test->add(BOOST_TEST_CASE(test_algorithm_transform_memberptr));
	test->add(BOOST_TEST_CASE(test_algorithm_transform));
	test->add(BOOST_TEST_CASE(test_algorithm_call));
	test->add(BOOST_TEST_CASE(test_factory));
	test->add(BOOST_TEST_CASE(test_algorithm));
	test->add(BOOST_TEST_CASE(test_inherit));
	test->add(BOOST_TEST_CASE(test_iterator_cast));
	test->add(BOOST_TEST_CASE(test_iterator_n));
	test->add(BOOST_TEST_CASE(test_iterator_traits));
	test->add(BOOST_TEST_CASE(test_iterator_virtual));
	test->add(BOOST_TEST_CASE(test_math_fixed));
	test->add(BOOST_TEST_CASE(test_math_vector));
	test->add(BOOST_TEST_CASE(test_multitype));
	test->add(BOOST_TEST_CASE(test_multi_iterator));
	test->add(BOOST_TEST_CASE(test_range));
	test->add(BOOST_TEST_CASE(test_range_numeric_iterator));
	test->add(BOOST_TEST_CASE(test_template_crtp));
	test->add(BOOST_TEST_CASE(test_template_group));
	test->add(BOOST_TEST_CASE(test_template_tt));
	test->add(BOOST_TEST_CASE(test_traits_processor));

#if defined( INDUSTRY_OS_WINDOWS )
	test->add(BOOST_TEST_CASE(test_api_windows_registry));

#elif defined( INDUSTRY_OS_X )
	//....

#endif

	return test;
}
