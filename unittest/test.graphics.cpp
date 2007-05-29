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
	color< unsigned char, rgba > c1;
	color< packed8888, rgba > c2;
	c1.red = 42;
	c2.red() = 42;
}
