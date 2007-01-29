// Copyright (c) 2006-2007 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <industry/languages/ruby/ruby.hpp>

#ifdef _M_X64
#pragma message( "X64 platform not supported." )
void test_languages_ruby() {}
#else
using namespace industry::languages::ruby;

namespace {
	struct Empty {
		int print_two(int j) const {
			std::cout<<"two! "<<j<<" "<<2+j<<std::endl;
			return 2 * j;
		}
	};

	void print_hello() {
		std::cout<<"Hello world"<<std::endl;
	}

	INDUSTRY_RUBY_MODULE(MyTestModule) {
		class_<Empty>("MyTestClass").
			def("Work", print_hello).
			def("WorkOther", &Empty::print_two);
	}
}

void test_languages_ruby() {
}
#endif
