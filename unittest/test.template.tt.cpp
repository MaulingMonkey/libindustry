// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  1, 2007 - Created

#include <industry/template/group.hpp>
#include <industry/template/tt.hpp>

namespace {
	template < typename > struct foo1 {};
	typedef ::industry::templates::group< ::industry::templates::tt1< foo1 > > foo1_group;
}

void test_template_tt() {
	foo1_group test1;
	
#ifdef __GNUC__
	test1 = test1;
#endif
}
