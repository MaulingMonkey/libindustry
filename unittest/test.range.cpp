// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - dot names to directory based name fixes
// Nov 12, 2006 - Created

#include <industry/arrays.hpp>
#include <industry/range.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

void test_range( void ) {
	using namespace industry;

	const int data1[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	int       data2[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	int       data3[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 1000 };
	int       small[] = { 1 , 2 , 3 };

	BOOST_CHECK( begin(data1) == make_range(data1).begin() );
	BOOST_CHECK( end(data1)   == make_range(data1).end  () );
	BOOST_CHECK( size(data1)  == make_range(data1).size () );
	
	BOOST_CHECK( make_range(data1) == make_range(data2) );
	BOOST_CHECK( make_range(data1) != make_range(small) );
	BOOST_CHECK( make_range(small) != make_range(data1) );
	BOOST_CHECK( make_range(data1) != make_range(data3) );

	BOOST_CHECK(( make_range(data1) == make_range(begin(data1),end(data1)) ));
	BOOST_CHECK(( make_range(data1  ,data1  ).empty() ));
	BOOST_CHECK(( make_range(data1  ,data1+0).empty() ));
	BOOST_CHECK(( make_range(data1+0,data1+0).empty() ));
	BOOST_CHECK(( make_range(data1,data1).size() == 0));
}
