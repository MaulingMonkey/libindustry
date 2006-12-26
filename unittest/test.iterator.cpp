// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - dot names to directory based name fixes
// Nov 12, 2006 - Created

#include <industry/iterator/n.hpp>
#include <industry/arrays.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

void test_iterator_n( void ) {
	using namespace industry;

	int data[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	std::vector< int > example( begin(data) , end(data) );

	std::vector< int > result( n_iterator< std::vector<int>::iterator >( example.begin() , 4 )
	                         , n_iterator< std::vector<int>::iterator >()
	                         );

	BOOST_REQUIRE( result.size() == 4 );
	for ( int i = 0 ; i < 4 ; ++i ) BOOST_CHECK( result[i] == i+1 );
	BOOST_CHECK( std::equal( n_iterator< int* >( begin(data) , 4 ) , n_iterator< int* >() , result.begin() ) );
}
