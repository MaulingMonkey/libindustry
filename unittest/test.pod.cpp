// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/pod/tuple.hpp>
#include <industry/traits/pod.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/tuple/tuple.hpp>

namespace {
	struct pod1 { int a, b, c; };

	struct nonpod1 {
		typedef pod1 pod_type;

		int a, b, c;
		nonpod1( int a, int b, int c ): a(a), b(b), c(c) {}
		nonpod1( const pod_type& p ): a(p.a), b(p.b), c(p.c) {}
		
		operator pod_type() const { pod_type p = {a,b,c}; return p; }
	};

	struct nonpod2 {
		int a, b, c;
		nonpod2( int a, int b, int c ): a(a), b(b), c(c) {}
	};
}

namespace industry {
	namespace pod {
		template<> struct to_pod_conversion_traits< nonpod2 > {
			typedef pod1 pod_type;
			static pod_type convert( const nonpod2& n ) { pod_type p = {n.a, n.b, n.c}; return p; }
		};
		template<> struct from_pod_conversion_traits< nonpod2 , pod1 > {
			static nonpod2 convert( const pod1& p ) { return nonpod2(p.a,p.b,p.c); }
		};
	}
}

void test_pod( void ) {
	pod1    legal1 = {1, 2, 3};
	//nonpod1 illegal1 = {1, 2, 3}; //OK (error)
	//nonpod2 illegal2 = {1, 2, 3}; //OK (error)
	nonpod1 legal2( 1, 2, 3 );
	nonpod2 legal3( 1, 2, 3 );



	industry::pod::tuple< int , float , char > legal4 = { 1, 2.3f, 'a' };
	industry::pod::tuple< pod1, nonpod1, nonpod2 > legal5 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//industry::pod::tuple< pod1, nonpod1, nonpod2 > illegal3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 }; //OK (error)


	BOOST_CHECK_EQUAL( legal4.element2   , 'a' );
	BOOST_CHECK_EQUAL( legal5.element1.b ,  5  );
	BOOST_CHECK_EQUAL( legal5.element2.a ,  7  );


	industry::pod::tuple< pod1, nonpod1, nonpod2 > legal6[] = {
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	};
	boost::tuples::tuple< pod1, nonpod1, nonpod2 > conversion1 = legal6[0].unpod();
	boost::tuples::tuple< pod1, nonpod1, nonpod2 > conversion2 = legal6[1];


	BOOST_CHECK_EQUAL( legal6[1].element1.b   , 5 );
	BOOST_CHECK_EQUAL( conversion1.get<1>().b , 5 );
	BOOST_CHECK_EQUAL( conversion2.get<1>().c , 6 );

	industry::pod::tuple< const char*, const char* > legal7[] = {
		{ "Panda likes", "Pineapples" },
		{ "Panda eats", "Bamboo" },
	};

	std::string pineapple, eats;
	boost::tuples::tie( boost::tuples::ignore , pineapple ) = legal7[0].unpod();
	//boost::tuples::tie( eats , boost::tuples::ignore ) = legal7[1]; // BAD1  (cannot get this to work, use the above instead)

	BOOST_CHECK_EQUAL( pineapple , "Pineapples" );
	//BOOST_CHECK_EQUAL( eats , "Panda eats" ); // depends on BAD1 above

	boost::tuples::tuple< std::string , std::string > conversion3[] = { legal7[0] , legal7[1] };
	BOOST_CHECK_EQUAL( conversion3[0].get<0>() , "Panda likes" );
	BOOST_CHECK_EQUAL( conversion3[1].get<1>() , "Bamboo" );
}
