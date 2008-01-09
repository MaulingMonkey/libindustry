// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - dot names to directory based name fixes
// Jul 08, 2006 - Switched to using the Boost Unit Test Framework
// May 20, 2006 - Created

#include <industry/multitype.hpp>
#include <boost/test/unit_test.hpp>

void test_multitype( void ) {
	using namespace industry;
	
	multitype< float , unsigned > value;
	value = 42.0f;
	value = 3141u;
	
	float    f = value;
	unsigned u = value;
	
	BOOST_CHECK( f == 42.0f );
	BOOST_CHECK( u == 3141u );
}
