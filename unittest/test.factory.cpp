// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - Dot to directory filename categorization fixes for SVN
// Sep 22, 2006 - Added ctor-selection checks ("ctor_method[_used]")
// Jul 08, 2006 - Switched to using the Boost Unit Test Framework
// May 31, 2006 - Some refactoring, templatized test function and tested for all 5 currently supported types
// May 31, 2006 - Re-enabled invalid type test (throws instead of asserts now)
// May 27, 2006 - Disabled intentional assert, added void(void) to the test
// May 21, 2006 - Created

#include <industry/factory.hpp>
#include <boost/array.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cassert>

namespace {
	struct base {
		virtual ~base() {}

		typedef int ctor_method;
		static const ctor_method
			void_ctor_method    = 1,
			int_ctor_method     = 2,
			char_ctor_method    = 3,
			int_int_ctor_method = 4,
			copy_ctor_method    = 5;
	};

	class foo : public base {
	public:
		ctor_method ctor_method_used;

		foo( void )          : base(), ctor_method_used( void_ctor_method    ) {}
		foo( int )           : base(), ctor_method_used( int_ctor_method     ) {}
		foo( char )          : base(), ctor_method_used( char_ctor_method    ) {}
		foo( int , int )     : base(), ctor_method_used( int_int_ctor_method ) {}
		foo( const foo & f ) : base(), ctor_method_used( f.ctor_method_used  ) {}
	};
	class bar : public base {
	public:
		ctor_method ctor_method_used;

		bar( void )          : base(), ctor_method_used( void_ctor_method    ) {}
		bar( int )           : base(), ctor_method_used( int_ctor_method     ) {}
		bar( char )          : base(), ctor_method_used( char_ctor_method    ) {}
		bar( int , int )     : base(), ctor_method_used( int_int_ctor_method ) {}
		bar( const bar & b ) : base(), ctor_method_used( b.ctor_method_used  ) {}
	};
}

template < typename T > void verify( const boost::any & any , base::ctor_method ctor_method_used ) {
	bool threw = true;
	BOOST_CHECK_NO_THROW(( boost::any_cast< T >( any ) , threw = false ));
	if (!threw) BOOST_CHECK_EQUAL( boost::any_cast< T >( any ).ctor_method_used , ctor_method_used );
}

template < typename T , typename B > void verify( const boost::shared_ptr<B> & ptr , base::ctor_method ctor_method_used ) {
	const T * p = 0;
	BOOST_CHECK( p = dynamic_cast< T* >( ptr.get() ) );
	if (p) BOOST_CHECK_EQUAL( p->ctor_method_used , ctor_method_used );
}

template < typename T , typename B > void verify( const std::auto_ptr<B> & ptr , base::ctor_method ctor_method_used ) {
	const T * p = 0;
	BOOST_CHECK( p = dynamic_cast< T* >( ptr.get() ) );
	if (p) BOOST_CHECK_EQUAL( p->ctor_method_used , ctor_method_used );
}

template < typename T , BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , typename V ) >
void verify( const boost::variant< BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , V ) > & variant , base::ctor_method ctor_method_used ) {
	const T * p = 0;
	BOOST_CHECK_NO_THROW( p = &(boost::get< T >( variant )) );
	if (p) BOOST_CHECK_EQUAL( p->ctor_method_used , ctor_method_used );
}

template < typename T , typename B > void verify( B * ptr , base::ctor_method ctor_method_used ) {
	const T * p = 0;
	BOOST_CHECK( p = dynamic_cast< T* >( ptr ) );
	if (p) BOOST_CHECK_EQUAL( p->ctor_method_used , ctor_method_used );
}

template < typename T > void cleanup(       T * raw_ptr ) { delete raw_ptr; }
template < typename T > void cleanup( const T * raw_ptr ) { delete raw_ptr; }
template < typename T > void cleanup( const T & ) {}

template < typename InterfaceT >
void test_factory_interface( void ) {
	industry::factory< industry::type , InterfaceT , industry::methods< void ( void ) , void ( int ) , void( char ) , void ( int , int ) > > factory_via_methods;
	industry::factory< industry::type , InterfaceT ,                    void ( void ) , void ( int ) , void( char ) , void ( int , int )   > factory;
	factory.template type< foo >( typeid(foo) );
	factory.template auto_type< bar >();

	boost::array< industry::type , 2 > types = { typeid(foo) , typeid(bar) };
	boost::array< InterfaceT , 2 > example;
	for ( unsigned i = 0 ; i < 2 ; ++i ) {
		example[i] = factory.create( types[i] , 42 );
	}

	verify< foo >( example[0] , base::int_ctor_method );
	verify< bar >( example[1] , base::int_ctor_method );

	InterfaceT v  = factory.create( typeid(foo)           );
	InterfaceT u  = factory.create( typeid(foo) , 42      );
	InterfaceT c  = factory.create( typeid(foo) , 'c'     );
	InterfaceT uu = factory.create( typeid(foo) , 42 , 42 );

	verify< foo >( v  , base::void_ctor_method    );
	verify< foo >( u  , base::int_ctor_method     );
	verify< foo >( c  , base::char_ctor_method    );
	verify< foo >( uu , base::int_int_ctor_method );

	BOOST_CHECK_THROW( factory.create(typeid(int),42) , industry::bad_factory_type );
	cleanup( example[0] );
	cleanup( example[1] );
	cleanup( v );
	cleanup( u );
	cleanup( c );
	cleanup( uu );
}

void test_factory( void ) {
	test_factory_interface< boost::any                  >();
	test_factory_interface< std::auto_ptr< base >       >();
	test_factory_interface< boost::shared_ptr< base >   >();
	test_factory_interface< boost::variant< foo , bar > >();
	test_factory_interface< base *                      >();
}
