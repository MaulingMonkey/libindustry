// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_STATIC_ASSERT
#define IG_INDUSTRY_STATIC_ASSERT

#include <boost/static_assert.hpp>

#define INDUSTRY_STATIC_ASSERT(x) BOOST_STATIC_ASSERT(x)

namespace industry {
	namespace STATIC_ASSERTION_FAILURES {
		template < template < typename > class testee > struct TEMPLATE1_DOES_NOT_EXIST {};
	}
}

#define INDUSTRY_STATIC_ASSERT_TEMPLATE1_EXISTS(t) \
   typedef ::boost::static_assert_test<\
      sizeof(::industry::STATIC_ASSERTION_FAILURES::TEMPLATE1_DOES_NOT_EXIST< t >)>\
         BOOST_JOIN(boost_static_assert_typedef_, __LINE__)

#endif //ndef IG_INDUSTRY_STATIC_ASSERT
