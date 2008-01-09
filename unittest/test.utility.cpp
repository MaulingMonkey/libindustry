// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/utility/tuple_accessors.hpp>
#include <industry/utility/new_shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits.hpp>

namespace {
	template < typename T > struct is_a {
		template < typename U > struct f {
			enum { value = boost::is_same< T, U >::value };
		};
	};

	struct foo {
		int a;
		const int &b;
		int &c;

		foo( int a, const int& b, int& c ): a(a), b(b), c(c) {}
	};

	boost::shared_ptr<foo> bar( const boost::shared_ptr< foo > & f ) { return f; }
}

void test_utility( void ) {
	using namespace industry::utility;

	int a=1, b=2, c=3;
	boost::shared_ptr<foo> ptr = bar(new_shared_ptr< foo >( a,b,boost::ref(c) ));
	BOOST_CHECK_EQUAL(a,ptr->a);
	BOOST_CHECK_EQUAL(b,ptr->b);
	BOOST_CHECK_EQUAL(c,ptr->c);
	a=4, b=5, c=6;
	BOOST_CHECK_EQUAL(1,ptr->a);
	BOOST_CHECK_EQUAL(b,ptr->b);
	BOOST_CHECK_EQUAL(c,ptr->c);


	boost::tuple< char, int, float, short > legal1( 'a', 42, 3.1415f , 1 );
	BOOST_CHECK_EQUAL( legal1.get<0>() , get_at_type< char  >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , get_at_type< int   >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , get_at_type< short >( legal1 ) );
	BOOST_CHECK_EQUAL( legal1.get<0>() , get_at_type_or_default< char  >( legal1 , 'b'  ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , get_at_type_or_default< int   >( legal1 , 24   ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , get_at_type_or_default< short >( legal1 , -1   ) );
	BOOST_CHECK_EQUAL(   4242          , get_at_type_or_default< long  >( legal1 , 4242 ) );
	
	BOOST_CHECK_EQUAL( legal1.get<0>() , (get_if_type< is_a< char  >::f >( legal1 ) ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , (get_if_type< is_a< int   >::f >( legal1 ) ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , (get_if_type< is_a< short >::f >( legal1 ) ) );
	BOOST_CHECK_EQUAL( legal1.get<0>() , (get_if_type_or_default< is_a< char  >::f >( legal1 , 4242 ) ) );
	BOOST_CHECK_EQUAL( legal1.get<1>() , (get_if_type_or_default< is_a< int   >::f >( legal1 , 4242 ) ) );
	BOOST_CHECK_EQUAL( legal1.get<3>() , (get_if_type_or_default< is_a< short >::f >( legal1 , 4242 ) ) );
	BOOST_CHECK_EQUAL(   4242          , (get_if_type_or_default< is_a< long  >::f >( legal1 , 4242 ) ) );

	//get_at_type< long >( legal1 ); // OK (clean error)

	//get_if_type< is_a< long >::f >( legal1 ); // OK (cleanish error)


	boost::tuple< char, char >      reoccurance1( 'a', 'a' );
	boost::tuple< char, int, char > reoccurance2( 'a', 42, 'a' );
	boost::tuple< int, char, char > reoccurance3( 42, 'a', 'a' );
	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , get_at_type< int >( reoccurance2 ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , get_at_type< int >( reoccurance3 ) );
	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , get_at_type_or_default< int  >( reoccurance2 ,   24 ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , get_at_type_or_default< int  >( reoccurance3 ,   24 ) );
	BOOST_CHECK_EQUAL(   4242                , get_at_type_or_default< long >( reoccurance3 , 4242 ) );

	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , (get_if_type< is_a< int >::f >( reoccurance2 ) ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , (get_if_type< is_a< int >::f >( reoccurance3 ) ) );
	BOOST_CHECK_EQUAL( reoccurance2.get<1>() , (get_if_type_or_default< is_a< int  >::f >( reoccurance2 ,   24 ) ) );
	BOOST_CHECK_EQUAL( reoccurance3.get<0>() , (get_if_type_or_default< is_a< int  >::f >( reoccurance3 ,   24 ) ) );
	BOOST_CHECK_EQUAL(   4242                , (get_if_type_or_default< is_a< long >::f >( reoccurance3 , 4242 ) ) );

	//get_at_type< char >( reoccurance1 ); //OK (clean error)
	//get_at_type< char >( reoccurance2 ); //OK (clean error)
	//get_at_type< char >( reoccurance3 ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance1 , 'a' ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance2 , 'a' ); //OK (clean error)
	//get_at_type_or_default< char >( reoccurance3 , 'a' ); //OK (clean error)

	//get_if_type< is_a< char  >::f >( reoccurance1 ); //OK (clean error)
	//get_if_type< is_a< char  >::f >( reoccurance2 ); //OK (clean error)
	//get_if_type< is_a< char  >::f >( reoccurance3 ); //OK (clean error)
	//get_if_type_or_default< is_a< char  >::f >( reoccurance1 , 'a' ); //OK (clean error)
	//get_if_type_or_default< is_a< char  >::f >( reoccurance2 , 'a' ); //OK (clean error)
	//get_if_type_or_default< is_a< char  >::f >( reoccurance3 , 'a' ); //OK (clean error)
}
