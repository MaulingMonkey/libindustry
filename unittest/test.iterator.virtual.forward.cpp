// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 31, 2006 - Created

#include <industry/iterator/virtual_forward.hpp>
#include <industry/arrays.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

void test_iterator_virtual_forward() {
	using namespace industry;
	
	typedef industry::virtual_forward_iterator< int >       int_viter;
	typedef industry::virtual_forward_iterator< const int > const_int_viter;

	int data[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	std::vector< int > example( begin(data) , end(data) );
	BOOST_CHECK( std::equal( example.begin() , example.end() , int_viter(example.begin()) ) );
	BOOST_CHECK( std::equal( int_viter(example.begin()) , int_viter(example.end()) , example.begin() ) );
	
	BOOST_CHECK( std::equal( example.begin() , example.end() , const_int_viter(example.begin()) ) );
	BOOST_CHECK( std::equal( const_int_viter(example.begin()) , const_int_viter(example.end()) , example.begin() ) );
	
	const std::vector< int > & const_example = example;
	BOOST_CHECK( std::equal( const_example.begin() , const_example.end() , const_int_viter(const_example.begin()) ) );
	BOOST_CHECK( std::equal( const_int_viter(const_example.begin()) , const_int_viter(const_example.end()) , const_example.begin() ) );
	
	const int_viter iv_2(example.begin()+2);
	int_viter iv_5 = iv_2;
	std::advance( iv_5 , 3 );
	BOOST_CHECK_EQUAL( *iv_5 , 6 );
}
