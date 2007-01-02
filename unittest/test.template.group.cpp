// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 31, 2006 - Created

#include <industry/template/group.hpp>
#include <boost/test/unit_test.hpp>

struct foo {};
struct bar {};
struct baz {};
template < typename > struct foo1 {};
template < typename > struct bar1 {};
template < typename > struct baz1 {};
template < typename , typename > struct foo2 {};
template < typename , typename > struct bar2 {};
template < typename , typename > struct baz2 {};

typedef industry::group < foo  , bar  , baz  > group;
typedef industry::tt1_group< foo1 , bar1 , baz1 > group1;
typedef industry::tt2_group< foo2 , bar2 , baz2 > group2;

void test_template_group() {
	group a;
	group1 b;
	group2 c;
	
#ifdef __GNUC__
	a = a , b = b , c = c; //quash "unused" warnings
#endif
}
