// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/traits/container.hpp>
#include <industry/range.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/static_assert.hpp>
#include <map>
#include <vector>
#include <cstddef>

namespace {
	struct not_a_container1 {};

	struct not_a_container2 {
		//all the container typedefs:

		typedef int value_type;
		typedef int* iterator;
		typedef const int* const_iterator;
		typedef int& reference;
		typedef const int& const_reference;
		typedef int* pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t size_type;
	};
}

void test_traits_container() {
	using namespace industry;

	BOOST_STATIC_ASSERT((  is_a_container<       std::vector< int > >::value ));
	BOOST_STATIC_ASSERT((  is_a_container< const std::vector< int > >::value ));
	BOOST_STATIC_ASSERT((  is_a_container< const std::map< char , int > >::value ));
	BOOST_STATIC_ASSERT(( !is_a_container< int >::value ));
	BOOST_STATIC_ASSERT(( !is_a_container< not_a_container1 >::value ));

	// Compile failure tests (should be checked 1 by 1):
	// Note:  Uncommented lines indicate currently failing-to-fail code
	// Last Tested:  Jan 23, 2007 @ 8:02am by pandamojo
	//
	// BOOST_STATIC_ASSERT((  is_a_container< not_a_container2 >::value ));
	//
	// Note:  End of uncommented-as-failing-to-fail code sectio
}
