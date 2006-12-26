// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - dot names to directory based name fixes
// Nov 12, 2006 - Created

#include <industry/algorithm.hpp>
#include <industry/arrays.hpp>
#include <industry/range.hpp>
#include <boost/test/unit_test.hpp>
#include <iterator>
#include <vector>

namespace {
	inline bool is_odd( unsigned value )  { return  (value & 0x1); }
	inline bool is_even( unsigned value ) { return !(value & 0x1); }
}

void test_algorithm( void ) {
	using namespace industry;

	unsigned data[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	unsigned expected_results[] = { 1 , 3 , 5 , 7 };

	const std::vector< unsigned > data2( begin(data) , end(data) );
	std::vector< unsigned > results;
	
	make_range(data) | filter( is_odd )
	                 | push_back(results)
	                 ;

	BOOST_CHECK(( make_range(results) == make_range(expected_results) ));

	data2 | filter( is_even )
	      | push_back( results )
	      ;
}
