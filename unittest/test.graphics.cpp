// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <boost/test/unit_test.hpp>
#include <industry/graphics/color.hpp>
#include <industry/graphics/image.hpp>

namespace {
}
void test_graphics() {
	using namespace industry::graphics;
	color< unsigned char, rgba > d1, c1(1,2,3,4);
	color< packed8888   , rgba > d2, c2(1,2,3,4);
	color< packed565    , rgb  > d3, c3(1,2,3);
	color< packed5551   , rgba > d4, c4(1,2,3,1);
	c1.red     = 42;
	c2.red()   = 42;
	c3.green() = 12;
	c4.alpha() = 0;

	BOOST_CHECK_EQUAL( c1.red     , 42 );
	BOOST_CHECK_EQUAL( c1.green   ,  2 );
	BOOST_CHECK_EQUAL( c1.blue    ,  3 );
	BOOST_CHECK_EQUAL( c1.alpha   ,  4 );
	BOOST_CHECK_EQUAL( c2.red()   , 42 );
	BOOST_CHECK_EQUAL( c2.green() ,  2 );
	BOOST_CHECK_EQUAL( c2.blue()  ,  3 );
	BOOST_CHECK_EQUAL( c2.alpha() ,  4 );
	BOOST_CHECK_EQUAL( c3.red()   ,  1 );
	BOOST_CHECK_EQUAL( c3.green() , 12 );
	BOOST_CHECK_EQUAL( c3.blue()  ,  3 );
	BOOST_CHECK_EQUAL( c4.red()   ,  1 );
	BOOST_CHECK_EQUAL( c4.green() ,  2 );
	BOOST_CHECK_EQUAL( c4.blue()  ,  3 );
	BOOST_CHECK_EQUAL( c4.alpha() ,  0 );
}
