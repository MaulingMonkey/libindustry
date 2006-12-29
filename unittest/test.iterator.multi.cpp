// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 27, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#include <industry/algorithm.hpp>
#include <industry/iterator/multi.hpp>
#include <industry/range.hpp>
#include <list>
#include <vector>
#include <boost/test/unit_test.hpp>

namespace {
	void add_one(int& i) {
		++i;
	}
}

void test_multi_iterator() {
	typedef int*                         Iter1;
	typedef std::vector<int>::iterator   Iter2;
	typedef std::list<int>::iterator     Iter3;
		
	industry::multi_iterator< Iter1 , Iter2 > iter12_end;
	industry::multi_iterator< Iter1 , Iter3 > iter13_end;
	industry::multi_iterator< Iter2 , Iter3 > iter23_end;

	std::vector<int> v(5);
	std::list<int> l(5);
	int expected_result[10];
	std::fill_n(expected_result, 10, 1);

	industry::make_range(v) + l;

	industry::inorder(v, l) | industry::call(add_one);

	BOOST_CHECK(( industry::inorder(v, l) == industry::make_range(expected_result) ));
}
