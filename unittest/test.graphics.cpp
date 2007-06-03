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

void test_graphics() {
	using namespace industry::graphics;

	rgba< unsigned char > d1, c1(1,2,3,4);
	packed565< rgb > d3, c3(1,2,3);

	c1.red     = 42;
	c3 = rgb<unsigned char>(42,2,3);

	BOOST_CHECK_EQUAL( c1.red     , 42 );
	BOOST_CHECK_EQUAL( c1.green   ,  2 );
	BOOST_CHECK_EQUAL( c1.blue    ,  3 );
	BOOST_CHECK_EQUAL( c1.alpha   ,  4 );

	BOOST_CHECK_EQUAL( rgb<unsigned char>(c3).red   , 42>>3<<3 );
	BOOST_CHECK_EQUAL( rgb<unsigned char>(c3).green ,  2>>2<<2 );
	BOOST_CHECK_EQUAL( rgb<unsigned char>(c3).blue  ,  3>>3<<3 );

	image< rgba< unsigned char > > i1(100,100), x1;
	image< packed565<rgb> >        i2(100,100), x2;
	const image< packed565<rgb> >  i3(100,100), x3;

	i1[1][2] = rgba<unsigned char>(1,2,3,4);
	i2[1][2] = rgb <unsigned char>(1,2,3  );
	// i3[1][2] = rgb <unsigned char>(1,2,3  ); // Should fail to compile -- OK, tested 6/2/07 --pandamojo
	rgba<unsigned char> p1 = i1[1][2];
	rgb <unsigned char> p2 = i2[1][2];
	rgb <unsigned char> p3 = i3[1][2];

	i1 = i2;
}
