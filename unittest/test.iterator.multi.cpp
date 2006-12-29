// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 27, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#include <industry/iterator/multi.hpp>
#include <industry/range.hpp>
#include <list>
#include <vector>

void test_multi_iterator() {
	typedef int*                         Iter1;
	typedef std::vector<int>::iterator   Iter2;
	typedef std::list<int>::iterator     Iter3;
		
	industry::multi_iterator< Iter1 , Iter2 > iter12_end;
	industry::multi_iterator< Iter1 , Iter3 > iter13_end;
	industry::multi_iterator< Iter2 , Iter3 > iter23_end;

	std::vector<int> v(10);
	std::list<int> l(10);

	industry::make_range(v) + l;

	industry::inorder(v, l);
}
