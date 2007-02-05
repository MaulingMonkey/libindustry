// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/algorithm/call.hpp>
#include <boost/bind.hpp>
#include <boost/test/unit_test.hpp>
#include <functional>

namespace {
	//Some free functions to test with:
	void expect_42( int v ) { BOOST_CHECK(( v == 42 )); }

	//Some member functions to test with:
	struct foo {
		void bar( void )       {}
		void baz( void ) const {}
	};
}

void test_algorithm_call() {
	using namespace industry::algorithm;

	//Testing signature matching stuff:
	call(expect_42), call(&foo::bar), call(&foo::baz), call(std::bind2nd(std::plus<int>(),42)), call(boost::bind(&foo::bar,_1));
}
