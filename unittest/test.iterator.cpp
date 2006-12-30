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

	std::vector< int > result1( n_iterator< std::vector<int>::iterator >( example.begin() )
	                          , n_iterator< std::vector<int>::iterator >( 4 )
	                          );
	std::vector< int > result2( n_iterator< std::vector<int>::iterator >( example.begin() )
	                          , n_iterator< std::vector<int>::iterator >( example.end() , 4 )
	                          );
	std::vector< int > result3( n_iterator< std::vector<int>::iterator >( example.begin() )
	                          , n_iterator< std::vector<int>::iterator >( example.end() , 12 )
	                          );
	BOOST_CHECK( result1.size() == 4 && std::equal(result1.begin(),result1.end(),begin(data)) );
	BOOST_CHECK( result2.size() == 4 && std::equal(result2.begin(),result2.end(),begin(data)) );
	BOOST_CHECK( result3.size() == 8 && std::equal(result3.begin(),result3.end(),begin(data)) );
}
