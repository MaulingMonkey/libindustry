// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - dot names to directory based name fixes
// Jul 08, 2006 - Switched to using the Boost Unit Test Framework
// May 20, 2006 - Created

#include <industry/inherit.hpp>
#include <industry/nil.hpp>
#include <boost/test/unit_test.hpp>

struct foo {};
struct bar {};
struct baz : industry::inherit< foo , bar , industry::nil , industry::nil > { virtual ~baz(){} };

void test_inherit( void ) {
	baz test;
	BOOST_CHECK(  dynamic_cast<           foo * >( &test ) );
	BOOST_CHECK(  dynamic_cast<           bar * >( &test ) );
	BOOST_CHECK( !dynamic_cast< industry::nil * >( (baz*) &test ) ); //[1]
}

/****
 * 1. (baz*) cast supresses GCC warning that the cast cannot succeed
 *    (we know this, we're unit-test verifying this after all!!!)
****/
