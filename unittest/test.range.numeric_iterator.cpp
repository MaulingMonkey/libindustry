// Copyright (c) 2006-2007 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/iterator/numeric.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

void test_range_numeric_iterator( ) {
	using namespace industry;

	numeric_iterator<float> inf;
	numeric_iterator<float> begin(1.0, 1.0);

	BOOST_CHECK(( std::numeric_limits<ptrdiff_t>::max() == std::distance(begin, inf) ));
	BOOST_CHECK(( std::numeric_limits<ptrdiff_t>::min() == std::distance(inf, begin) ));
	BOOST_CHECK(( *(++begin) == 2.0));
	BOOST_CHECK(( *(begin++) == 2.0));
}
