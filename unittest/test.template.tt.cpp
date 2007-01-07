// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  1, 2007 - Created
// $LastChangedBy$ - $LastChangedDate$

#include <industry/template/group.hpp>
#include <industry/template/tt.hpp>

namespace {
	template < typename > struct foo1 {};
	template < typename , typename > struct foo2 {};
	template < typename , typename , typename > struct foo3 {};
	
	typedef ::industry::templates::group< ::industry::templates::tt1< foo1 > > foo1_group_1;
	typedef ::industry::group< ::industry::tt1< foo1 > > foo1_group_2;
	typedef ::industry::group< ::industry::tt2< foo2 > > foo2_group;
	typedef ::industry::group< ::industry::tt3< foo3 > > foo3_group;
}

void test_template_tt() {
	foo1_group_1 test1;
	foo1_group_2 test2;
	foo2_group   test3;
	foo3_group   test4;

	test1 = test1, test2 = test2, test3 = test3, test4 = test4;
}
