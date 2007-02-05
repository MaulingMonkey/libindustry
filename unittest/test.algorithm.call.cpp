// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/algorithm/call.hpp>
#include <boost/bind.hpp>
#include <functional>

namespace {
	//Some free functions to test with:
	int fourty_two() { return 42; }
	int& pass( int& a ) { return a; }
	const int& cpass( int& a ) { return a; }
	void prawn( int& changee ) { changee = 42; }

	//Some member functions to test with:
	struct foo {
		int bar( char v )       { return v; }
		int baz( char v ) const { return v; }
	};
}

void test_algorithm_call() {
	using namespace industry::algorithm;
	call( fourty_two ), call( pass ), call( cpass ), call( prawn );
	call( &foo::bar ), call( &foo::baz );
}
