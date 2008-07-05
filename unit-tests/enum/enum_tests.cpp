// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/enum.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace {
	INDUSTRY_ENUM( day, (is)(os), (sunday)(monday)(tuesday)(wednesday)(thursday)(friday)(saturday) );
}

BOOST_AUTO_TEST_CASE( test_enum ) {
	const day day1 = day::monday;
	const day day2 = day::tuesday;
	
	BOOST_CHECK_EQUAL( day1.value, day::monday  );
	BOOST_CHECK_EQUAL( day2.value, day::tuesday );

	BOOST_CHECK_EQUAL( day1       , day1        );
	BOOST_CHECK_EQUAL( day1       , day::monday );
	BOOST_CHECK_EQUAL( day::monday, day1        );

	BOOST_CHECK( day1         != day2         );
	BOOST_CHECK( day::tuesday != day1         );
	BOOST_CHECK( day1         != day::tuesday );

	BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(day1), "monday"    );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("monday"), day::monday );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("mOnDaY"), day::monday );

	BOOST_CHECK_THROW( boost::lexical_cast<day>("monda")  , boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("mondayz"), boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("monday "), boost::bad_lexical_cast );
}
