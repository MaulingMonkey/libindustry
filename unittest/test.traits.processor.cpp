// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/traits/processor.hpp>
#include <industry/range.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>

namespace {
	struct malformed_processor_1 { typedef industry::unbound_processor_tag processor_category; /* missing after<> template */ };
	struct malformed_processor_2 { typedef industry::bound_processor_tag   processor_category; /* missing iterator */ };
}

void test_traits_processor() {
	using namespace industry;

	// Bound processor compile-and-verification checks:
	BOOST_CHECK(( typeid(processor_traits<       std::vector<      int> >::processor_category) == typeid(bound_processor_tag) ));
	BOOST_CHECK(( typeid(processor_traits< const std::vector<      int> >::processor_category) == typeid(bound_processor_tag) ));
	//BOOST_CHECK(( typeid(processor_traits<       std::vector<const int> >::processor_category) == typeid(bound_processor_tag) )); //Illegal?  GCC does not grok
	//BOOST_CHECK(( typeid(processor_traits< const std::vector<const int> >::processor_category) == typeid(bound_processor_tag) )); //Illegal?  GCC does not grok

	BOOST_CHECK(( typeid(processor_traits<       int[5] >::processor_category) == typeid(bound_processor_tag) ));
	BOOST_CHECK(( typeid(processor_traits< const int[5] >::processor_category) == typeid(bound_processor_tag) ));

	// Compile failure tests (should be checked 1 by 1):
	// Note:  Uncommented lines indicate currently failing-to-fail code
	// Last Tested:  Jan 20, 2007 @ 1:39am by pandamojo
	//
	// processor_traits< int[] > err_1; err_1=err_1;
	// processor_traits< malformed_processor_1 > err_2; err_2=err_2;
	// processor_traits< malformed_processor_2 > err_3; err3=err_3;
	//
	// Note:  End of uncommented-as-failing-to-fail code section
}
