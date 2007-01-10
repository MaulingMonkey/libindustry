// Copyright (c) 2006-2007 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/algorithm/standard_algorithm_adaptors.hpp>
#include <industry/range.hpp>

void test_algorithm_standard_algorithm_adaptors() {
	using namespace industry;

	int numbers[] = {1, 2, 3, 4, 1, 2, 3, 4, 1};

	industry::count(numbers, 1);
	industry::fill_n(numbers, 9, 1);
	industry::count(numbers, 1);
}
