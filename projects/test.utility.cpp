// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/utility/tuple_accessors.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/tuple/tuple.hpp>

namespace {
}

void test_utility( void ) {
	using namespace industry::utility;
	boost::tuple< char, int, float, short > legal1( 'a', 42, 3.1415f , 1 );
	BOOST_CHECK_EQUAL( legal1.get<0>() , get_at_type< char  >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , get_at_type< int   >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , get_at_type< short >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<0>() , get_at_type_or_default< char  >( legal1 , 'b'  ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , get_at_type_or_default< int   >( legal1 , 24   ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , get_at_type_or_default< short >( legal1 , -1   ) );
	BOOST_CHECK_EQUAL(   4242          , get_at_type_or_default< long  >( legal1 , 4242 ) );
	//get_at_type< long >( legal1 ); // OK (clean error)


	boost::tuple< char, char >      reoccurance1( 'a', 'a' );
	boost::tuple< char, int, char > reoccurance2( 'a', 42, 'a' );
	boost::tuple< int, char, char > reoccurance3( 42, 'a', 'a' );
	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , get_at_type< int >( reoccurance2 ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , get_at_type< int >( reoccurance3 ) );
	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , get_at_type_or_default< int  >( reoccurance2 ,   24 ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , get_at_type_or_default< int  >( reoccurance3 ,   24 ) );
	BOOST_CHECK_EQUAL(   4242                , get_at_type_or_default< long >( reoccurance3 , 4242 ) );
	//get_at_type< char >( reoccurance1 ); //OK (clean error)
	//get_at_type< char >( reoccurance2 ); //OK (clean error)
	//get_at_type< char >( reoccurance3 ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance1 , 'a' ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance2 , 'a' ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance3 , 'a' ); //OK (clean error)
}
