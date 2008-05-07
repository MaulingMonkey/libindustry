// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <industry/languages/ruby/ruby.hpp>

#ifdef _M_X64
#pragma message( "X64 platform not supported." )

#else
using namespace industry::languages::ruby;

namespace {
	unsigned int test_value;
	std::string name;

	struct MyTestClass {
		unsigned int inc;

		MyTestClass(): inc(2) {}

		void work2() const { test_value += inc; }

		int mul_by_inc(int j) const { return j*inc; }

		int multi_arg_mul(int x, int y) { return x * y; }

		void arr(std::string n) { name = n; }
	};

	void work1() {
		test_value += 1;
	}

	INDUSTRY_RUBY_MODULE(MyTestModule) {
		class_<MyTestClass>("MyTestClass").
			var("inc2", &MyTestClass::inc).
			def("work1", work1).
			def("work2", &MyTestClass::work2).
			var("inc", &MyTestClass::inc).
			def("mul_by_inc", &MyTestClass::mul_by_inc).
			const_("Multiplicand", 4).
			def("arr", &MyTestClass::arr).
			def("multi_arg_mul", &MyTestClass::multi_arg_mul);
	}

	// prevent Boost.Test from detecting GCed objects as leaks:
	struct cleaner { ~cleaner() { rb_eval_string( "GC.start" ); } } instance;
}

BOOST_AUTO_TEST_SUITE( ruby_tests )

BOOST_AUTO_TEST_CASE( basic_invocation_test )
{
	test_value = 0;
	Init_MyTestModule();
	BOOST_CHECK_EQUAL( test_value, 0 );
	rb_eval_string("MyTestClass.new.work1" );
	BOOST_CHECK_EQUAL( test_value, 1 );
	rb_eval_string("MyTestClass.new.work2" );
	BOOST_CHECK_EQUAL( test_value, 3 );
	rb_eval_string("MyTestClass.new.arr('hello')");
	BOOST_CHECK_EQUAL( name, "hello");
}

BOOST_AUTO_TEST_CASE( arguments_and_return_test )
{
	Init_MyTestModule();
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.inc")), 2);
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.inc = 5")), 5);
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.mul_by_inc(2)")), 4 );
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.mul_by_inc(3)")), 6 );
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.multi_arg_mul(3, 2)")), 6 );
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.mul_by_inc(MyTestClass::Multiplicand)")), 8 );
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
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("\
		test_class = MyTestClass.new\n\
		test_class.inc = 7\n\
		test_class.inc2\n\
		")), 7);
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("\
		test_class = MyTestClass.new\n\
		test_class.inc = 7\n\
		test_class.mul_by_inc(2)\n\
		")), 14);
}

BOOST_AUTO_TEST_CASE( value_and_eval ) {
	Init_MyTestModule();
	
	value instance = eval("MyTestClass.new");
	BOOST_CHECK_EQUAL( eval              ( "MyTestClass.new.mul_by_inc(2)" ).to<int>()     , 4 );
	BOOST_CHECK_EQUAL( eval<int>         ( "MyTestClass.new.mul_by_inc(2)" )               , 4 );
	BOOST_CHECK_EQUAL( eval<MyTestClass*>( "MyTestClass.new" )->  mul_by_inc  (2)          , 4 );
	BOOST_CHECK_EQUAL(                   ( instance           ->*"mul_by_inc")(2).to<int>(), 4 );

	BOOST_CHECK_EQUAL(     value(4), 4 );
	BOOST_CHECK( value(4) != 8 );
	BOOST_CHECK( value(4)-4 );
}

BOOST_AUTO_TEST_SUITE_END() // ruby_tests

#endif
