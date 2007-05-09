// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/config.hpp>
#include <boost/test/unit_test.hpp>
#include <industry/api/freetype.hpp>

void test_api_freetype() {
	using namespace industry::api::freetype;
	locator loc;
	face_info info;
	BOOST_CHECK_NO_THROW( info = loc.find_face_info( "14pt Courier New bold" ) );
	BOOST_CHECK_EQUAL( info.size     , 14    );
	BOOST_CHECK_EQUAL( info.filename , "%WINDIR%\\Fonts\\COURBD.TTF" );
	BOOST_CHECK_EQUAL( info.bold     , true  );
	BOOST_CHECK_EQUAL( info.italic   , false );
}
