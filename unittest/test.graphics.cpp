// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

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
}
