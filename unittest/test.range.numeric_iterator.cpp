// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/arrays.hpp>
#include <industry/range.hpp>
#include <industry/iterator/numeric.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <industry/algorithm.hpp>
#include <industry/math/fixed.hpp>
#include <boost/test/floating_point_comparison.hpp>

void test_range_numeric_iterator( ) {
	using namespace industry;
	
	typedef fixed<int, 6> fp6;

	int expected_result[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int expected_result_odd[] = {1, 3, 5, 7, 9};

	std::vector<int> results;
	make_numeric_range(1, 10) | push_back(results);

	BOOST_CHECK(( make_range(expected_result) == make_range(results) ));

	results.clear();

	make_numeric_range(1, 10, 2) | push_back(results);

	BOOST_CHECK(( make_range(expected_result_odd) == make_range(results) ));

	numeric_iterator<int> f(1, 3, 1);
	++f;
	++f;
	//++f; - SK: asserts if you attempt to go past the end.

	f = numeric_iterator<int>(1, 3, 3);
	++f;
	//++f; - SK: asserts if you attempt to go past the end.

 	std::vector<fp6> fp_results;
	make_numeric_range(fp6(0.0), fp6(1.0), fp6(0.1)) | push_back(fp_results);
	BOOST_CHECK(( fp_results.size() == 11 )); //Note fp6 representation will result in 11 elements, not 10.
}
