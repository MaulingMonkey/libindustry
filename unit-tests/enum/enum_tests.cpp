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
	INDUSTRY_ENUM( day, (is)(os)(duplicate_constants_in_outer_scope), (sunday)(monday)(tuesday)(wednesday)(thursday)(friday)(saturday) );

	bool overloaded_right( day ) { return true;  }
	bool overloaded_right( int ) { return false; }
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

	BOOST_CHECK( overloaded_right(day1) );
	BOOST_CHECK( overloaded_right(day::monday) );
}

BOOST_AUTO_TEST_CASE( test_enum_raw ) {
	const day day1 = day::_monday;
	const day day2 = day::_tuesday;
	
	BOOST_CHECK_EQUAL( day1.value, day::_monday  );
	BOOST_CHECK_EQUAL( day2.value, day::_tuesday );

	BOOST_CHECK_EQUAL( day1       , day1        );
	BOOST_CHECK_EQUAL( day1       , day::_monday );
	BOOST_CHECK_EQUAL( day::_monday, day1        );

	BOOST_CHECK( day1          != day2         );
	BOOST_CHECK( day::_tuesday != day1         );
	BOOST_CHECK( day1          != day::_tuesday );

	BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(day1), "monday"     );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("monday"), day::_monday );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("mOnDaY"), day::_monday );

	BOOST_CHECK_THROW( boost::lexical_cast<day>("monda")  , boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("mondayz"), boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("monday "), boost::bad_lexical_cast );

	BOOST_CHECK( overloaded_right(day1) );
	BOOST_CHECK(!overloaded_right(day::_monday) ); // We can't get this right since it's an actual enum.
}

BOOST_AUTO_TEST_CASE( test_enum_outer_scope ) {
	const day day1 = monday;
	const day day2 = tuesday;
	
	BOOST_CHECK_EQUAL( day1.value, monday  );
	BOOST_CHECK_EQUAL( day2.value, tuesday );

	BOOST_CHECK_EQUAL( day1  , day1   );
	BOOST_CHECK_EQUAL( day1  , monday );
	BOOST_CHECK_EQUAL( monday, day1   );

	BOOST_CHECK( day1         != day2         );
	BOOST_CHECK( day::tuesday != day1         );
	BOOST_CHECK( day1         != day::tuesday );

	BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(day1), "monday"    );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("monday"), monday );
	BOOST_CHECK_EQUAL( boost::lexical_cast<day>    ("mOnDaY"), monday );

	BOOST_CHECK_THROW( boost::lexical_cast<day>("monda")  , boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("mondayz"), boost::bad_lexical_cast );
	BOOST_CHECK_THROW( boost::lexical_cast<day>("monday "), boost::bad_lexical_cast );

	BOOST_CHECK( overloaded_right(day1)   );
	BOOST_CHECK( overloaded_right(monday) );
}