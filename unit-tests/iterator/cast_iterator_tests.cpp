// Copyright (c) 2006-2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - dot names to directory based name fixes
// Nov 10, 2006 - Created

#include <industry/iterator/cast.hpp>
#include <boost/range.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <vector>

namespace {
	struct base { virtual ~base() {} };
	struct derived : base {};

	struct unrelated1 {};
	struct unrelated2 {};
}

BOOST_AUTO_TEST_CASE( test_const_iterator_cast ) {
	using namespace industry;

	base data[4];
	std::vector< const base * > source;
	for ( base * i = boost::begin( data ) ; i != boost::end( data ) ; ++i ) source.push_back( i );

	std::vector< base * > result( const_iterator_cast< base * >( source.begin() )
	                            , const_iterator_cast< base * >( source.end  () )
	                            );

	for ( unsigned i = 0 ; i != boost::size( data ) ; ++i ) {
		BOOST_CHECK_EQUAL( & data[i] , source[i] );
		BOOST_CHECK_EQUAL( source[i] , result[i] );
	}
}

BOOST_AUTO_TEST_CASE( test_dynamic_iterator_cast ) {
	using namespace industry;

	const unsigned size = 3;
	base    base_data   [size];
	derived derived_data[size];

	std::vector< base * > source;
	for ( unsigned i = 0 ; i < size ; ++i ) {
		source.push_back( base_data    + i );
		source.push_back( derived_data + i );
	}
	BOOST_REQUIRE( source.size() == size * 2 );

	std::vector< derived * > result( dynamic_iterator_cast< derived * >( source.begin() )
	                               , dynamic_iterator_cast< derived * >( source.end()   )
								   );
	BOOST_REQUIRE( result.size() == size * 2 );
	unsigned nulls = 0;
	for ( unsigned i = 0 ; i < result.size() ; ++i ) {
		if ( !result[i] ) {
			++nulls;
		} else {
			BOOST_CHECK( dynamic_cast< derived * >( result[i] ) );
		}
	}
	BOOST_CHECK( nulls == size );
}

BOOST_AUTO_TEST_CASE( test_reinterpret_iterator_cast ) {
	using namespace industry;

	unrelated1 data[4];
	std::vector< unrelated1 * > source;
	for ( unrelated1 * i = boost::begin( data ) ; i != boost::end( data ) ; ++i ) source.push_back( i );

	std::vector< unrelated2 * > result( reinterpret_iterator_cast< unrelated2 * >( source.begin() )
	                                  , reinterpret_iterator_cast< unrelated2 * >( source.end()   )
	                                  );

	for ( unsigned i = 0 ; i != boost::size( data ) ; ++i ) {
		BOOST_CHECK_EQUAL( & data[i] , source[i] );
		BOOST_CHECK_EQUAL( reinterpret_cast< unrelated2* >( source[i] ) , result[i] );
	}
}

BOOST_AUTO_TEST_CASE( test_static_iterator_cast ) {
	using namespace industry;

	derived data[4];
	std::vector< base *    > source;
	for ( derived * i = boost::begin( data ) ; i != boost::end( data ) ; ++i ) source.push_back( i );

	std::vector< derived * > result( static_iterator_cast< derived * >( source.begin() )
	                               , static_iterator_cast< derived * >( source.end()   )
	                               );

	for ( unsigned i = 0 ; i != boost::size( data ) ; ++i ) {
		BOOST_CHECK_EQUAL( & data[i] , source[i] );
		BOOST_CHECK_EQUAL( source[i] , result[i] );
	}
}
