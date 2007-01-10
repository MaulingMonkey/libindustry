// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 29, 2006 - New tests, refactoring
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
	std::vector< int > result4( n_iterator< std::vector<int>::iterator >( example.begin() )
	                          , n_iterator< std::vector<int>::iterator >( example.end() , 8 )
	                          );
	std::vector< int > result5( n_iterator< std::vector<int>::iterator >( example.begin() )
	                          , n_iterator< std::vector<int>::iterator >( example.end() , 0 )
	                          );
	std::vector< int > result6( n_iterator< int* >( begin(data) )
							  , n_iterator< int* >( end(data) , 4 )
							  );
	std::vector< int > result7( make_n_iterator( example.begin()   )
	                          , make_n_iterator( example.end() , 4 )
	                          );
	std::vector< int > result8( make_n_iterator( begin(data)   )
	                          , make_n_iterator( end(data) , 4 )
	                          );
	BOOST_CHECK( result1.size() == 4 && std::equal(result1.begin(),result1.end(),begin(data)) );
	BOOST_CHECK( result2.size() == 4 && std::equal(result2.begin(),result2.end(),begin(data)) );
	BOOST_CHECK( result3.size() == 8 && std::equal(result3.begin(),result3.end(),begin(data)) );
	BOOST_CHECK( result4.size() == 8 && std::equal(result4.begin(),result4.end(),begin(data)) );
	BOOST_CHECK( result5.size() == 0 );
	BOOST_CHECK( result6.size() == 4 && std::equal(result6.begin(),result6.end(),begin(data)) );
	BOOST_CHECK( result7.size() == 4 && std::equal(result7.begin(),result7.end(),begin(data)) );
	BOOST_CHECK( result8.size() == 4 && std::equal(result8.begin(),result8.end(),begin(data)) );
}
