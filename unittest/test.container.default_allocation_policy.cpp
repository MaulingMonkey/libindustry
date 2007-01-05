// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$


#include <industry/container/dynamic_array.hpp>
#include <boost/test/unit_test.hpp>

void test_container_default_allocation_policy( void ) {
	using namespace industry::container;

	dynamic_array_default_allocation_policy<int> policy;
	
	typedef dynamic_array_default_allocation_policy<int>::pointer pointer;
	typedef dynamic_array_default_allocation_policy<int>::size_type size_type;

	std::auto_ptr<int> p = std::auto_ptr<int>(policy.allocate(10));

	std::pair<pointer, size_type> r = policy.grow(10, p.get());
	BOOST_CHECK((r.second == 15));	
	delete r.first;
}
