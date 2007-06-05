// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Aug 17, 2006 - Created

#include <boost/test/unit_test.hpp>
#include <industry/api/devil/image.hpp>
#include <industry/graphics/image.hpp>

void test_api_devil() {
	using namespace industry::graphics;
	using namespace industry::api::devil;

	ilInit();
	BOOST_CHECK_THROW( load<rgb <unsigned char> >( "Testing 123" ), file_not_found );
	BOOST_CHECK_THROW( load<rgba<unsigned char> >( "Testing 123" ), file_not_found );

	boost::shared_ptr< image<rgb <unsigned char> > > i1;
	boost::shared_ptr< image<rgba<unsigned char> > > i2;
	BOOST_CHECK_NO_THROW( i1 = load<rgb <unsigned char> >( "..\\data\\libindustry.png" ) );
	BOOST_CHECK_NO_THROW( i2 = load<rgba<unsigned char> >( "..\\data\\libindustry.png" ) );

	if (i1) {
		BOOST_CHECK_EQUAL( i1->width()  , 800u );
		BOOST_CHECK_EQUAL( i1->height() , 600u );
	}
	if (i2) {
		BOOST_CHECK_EQUAL( i2->width()  , 800u );
		BOOST_CHECK_EQUAL( i2->height() , 600u );
	}
}
