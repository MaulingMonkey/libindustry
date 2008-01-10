// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <boost/test/auto_unit_test.hpp>
#include <industry/languages/ruby/ruby.hpp>

#ifdef _M_X64
#pragma message( "X64 platform not supported." )

#else
using namespace industry::languages::ruby;

namespace {
	unsigned int test_value;

	struct Empty {
		unsigned int inc;
		Empty(): inc(2) {}

		void work2() const { test_value += inc; }

		int mul_by_inc(int j) const { return j*inc; }
	};

	void work1() {
		test_value += 1;
	}

	INDUSTRY_RUBY_MODULE(MyTestModule) {
		class_<Empty>("MyTestClass").
			def("work1", work1).
			def("work2", &Empty::work2).
			def("mul_by_inc", &Empty::mul_by_inc);
	}
}

BOOST_AUTO_TEST_CASE( basic_invokation_test )
{
	test_value = 0;
	Init_MyTestModule();
	BOOST_CHECK_EQUAL( test_value, 0 );
	rb_eval_string("MyTestClass.new.work1" );
	BOOST_CHECK_EQUAL( test_value, 1 );
	rb_eval_string("MyTestClass.new.work2" );
	BOOST_CHECK_EQUAL( test_value, 3 );
	rb_eval_string("GC.start"); // prevent Boost.Test from detecting GCed objects as leaks
}

BOOST_AUTO_TEST_CASE( arguments_and_return_test )
{
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.mul_by_inc(2)")), 4 );
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.mul_by_inc(3)")), 6 );
	rb_eval_string("GC.start"); // prevent Boost.Test from detecting GCed objects as leaks
}

BOOST_AUTO_TEST_CASE( big_long_scripts ) {
	test_value = 0;
	Init_MyTestModule();
	rb_eval_string("\
		test_class = MyTestClass.new\n\
		test_class.work1\n\
		test_class.work2\n\
		");
	BOOST_CHECK_EQUAL( test_value, 3 );
	rb_eval_string("GC.start"); // prevent Boost.Test from detecting GCed objects as leaks
}

#endif
