// Copyright (c) 2006-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/config.hpp>

#define BOOST_TEST_MODULE     "libindustry unit tests"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>
#ifdef INDUSTRY_OS_LINUX
#include <boost/test/included/unit_test_framework.hpp>
#endif //def INDUSTRY_OS_LINUX
#include <industry/traits/function_traits.hpp>
