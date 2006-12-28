// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 27, 2006 - added test_multi_iterator
// Dec 26, 2006 - added test_factory
// Dec 25, 2006 - Many disabled tests (changing to using a directory structure once more for libindustry SVN on sourceforge)
// Jul 08, 2006 - Switched to using the Boost Unit Test Framework
// May 20, 2006 - Created

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4535 ) //calling _set_se_translator()
#pragma warning( disable : 4244 ) //possible initialization data loss conversion
#pragma warning( disable : 4267 ) //possible argument       data loss conversion
#endif

#include <industry/arrays.hpp>
#include <industry/config.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>
#include <iostream>
using boost::unit_test::test_suite;

#if defined( _MSC_VER )
#pragma warning(pop)
#endif

typedef void (*test_function_ptr)();

//A lot of currently not-fixed tests/etc
void test_algorithm();
void test_factory();
//void test_freetype();
//void test_image();
void test_inherit();
void test_iterator_cast();
void test_iterator_n();
void test_math_fixed();
void test_math_vector();
void test_multitype();
void test_multi_iterator();
//void test_nmap();
void test_range();
//void test_utility();
void test_range_numeric_iterator();
void test_algorithm_call();

static test_function_ptr test_functions[] = {
	test_algorithm_call,
	test_factory,
	test_algorithm,
	test_inherit,
	test_iterator_cast,
	test_iterator_n,
	test_math_fixed,
	test_math_vector,
	test_multitype,
	test_multi_iterator,
	test_range,
	test_range_numeric_iterator
};

#if defined( INDUSTRY_OS_WINDOWS )
//void test_win32_registry();
#endif

test_suite * init_unit_test_suite( int argc , char * argv[] ) {
	using namespace std;
	cout << "Test started..." << endl;

	test_suite * test = BOOST_TEST_SUITE( "libindustry master test suite" );

	for( unsigned i = 0 ; i < industry::arrays::size(test_functions) ; ++i ) {
		test->add(BOOST_TEST_CASE( test_functions[i] ));
	}

	//OS specific cases:
#if defined( INDUSTRY_OS_WINDOWS )
	//test->add( BOOST_TEST_CASE( & test_win32_registry ) );

#elif defined( INDUSTRY_OS_X )
	//....

#endif

	return test;
}
