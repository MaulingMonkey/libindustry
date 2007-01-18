// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 11, 2007 - Created

#include <industry/container.hpp>
#include <boost/test/unit_test.hpp>

namespace {
}

void test_container() {
	using namespace industry;

	int source[] = {1,2,3,4};
	std::vector< int > c;
	as_container(c) = source;
	BOOST_CHECK(( std::equal(c.begin(),c.end(),source) ));
}
