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
#include <functional>
#include <memory>
#include <iostream>

#ifdef _M_X64
#pragma message( "X64 platform not supported." )

#else
using namespace industry::languages::ruby;

#define TEST_ASSERT(t,e) struct e {}; assert<e,t>::value
namespace {
	template<class ErrorCode, bool Test> struct assert;
	template<class ErrorCode> struct assert<ErrorCode, true> { static const bool value = true; };

	unsigned int test_value;
	std::string name;

	struct MyTestClass {
		unsigned int inc;

		MyTestClass(): inc(2) {}

		void work2() const { test_value += inc; }

		int mul_by_inc(int j) const { return j*inc; }

		int multi_arg_mul(int x, int y) { return x * y; }

		void arr(const std::string& n) { name = n; }
	};

	class MyIntrusiveTestClass {
		mutable unsigned references;
	public:
		unsigned get_references() const { return references; }
		// TODO:  Make it so we can create/destroy our intrusive pointers to MITCs created on the Ruby side of things
		//        (Without exploding)

		MyIntrusiveTestClass(): references(0) {}
		MyIntrusiveTestClass( const MyIntrusiveTestClass& o ): references(0) {}

		friend void intrusive_ptr_add_ref( const MyIntrusiveTestClass* ptr ) { ++ptr->references; }
		friend void intrusive_ptr_release( const MyIntrusiveTestClass* ptr ) { if (!--ptr->references) delete ptr; }
	};

	class MyCopyableTestClass {
	public:
		std::size_t get_address() const { return (std::size_t)this; }
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

		class_<MyIntrusiveTestClass>("MyIntrusiveTestClass").
			def("references", &MyIntrusiveTestClass::get_references);

		class_<MyCopyableTestClass>("MyCopyableTestClass").
			def("address", &MyCopyableTestClass::get_address);
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
	BOOST_CHECK_EQUAL( NUM2UINT(rb_eval_string("MyTestClass.new.inc = 0x12345678")), 0x12345678);
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

	value array_test = eval("[1, 2, 3]");
	BOOST_CHECK_EQUAL(array_test[1], 2);
	BOOST_CHECK_EQUAL(array_test.length(), 3);
	BOOST_CHECK_EQUAL(     value(4), 4 );
	BOOST_CHECK( value(4) != 8 );
	BOOST_CHECK( value(4)-4 );

	value hash_test = eval("{'a' => 1, 'b' => 2, 'c' => 3}");
	BOOST_CHECK_EQUAL(hash_test["a"], 1);

	BOOST_CHECK_EQUAL(safe_eval<int>("2"), 2);
}

BOOST_AUTO_TEST_CASE( ownership_and_such ) {
	Init_MyTestModule();

	rb_eval_string("\
		class Inspector\n\
			def class_of( object )\n\
				object.class.to_s\n\
			end\n\
			def references_of( object )\n\
				object.references\n\
			end\n\
			def address_of( object )\n\
				object.address\n\
			end\n\
		end\n\
		");

	value i = eval("Inspector.new"); // give us something to (...->*"name")(...) from

	// Nontransfer of ownership
	std::auto_ptr<MyTestClass> mtc1( new MyTestClass() );
	BOOST_CHECK_EQUAL( "MyTestClass", (i->*"class_of")(mtc1.get()).to<std::string>() ); // raw pointer, shouldn't tranfser ownership
	BOOST_CHECK( mtc1.get() );


	// Transfer of ownership
	std::auto_ptr<MyTestClass> mtc2( new MyTestClass() );
	BOOST_CHECK_EQUAL( "MyTestClass", (i->*"class_of")(mtc2).to<std::string>() ); // directly passing auto_ptr, should transfer ownership
	BOOST_CHECK( !mtc2.get() );


	// Shared ownership
	boost::intrusive_ptr<MyIntrusiveTestClass> mitc1( new MyIntrusiveTestClass() );
	// FIXME: There's a temporary somewhere in there that's casuing references to be 3 on the line bellow:
	BOOST_CHECK_PREDICATE( std::greater_equal<int>(), ( (i->*"references_of")(mitc1).to<int>() )( 2 ) );
	//     ...This isn't a problem per se, but it disturbs me greatly not to know what's causing it  --pandamojo
	BOOST_WARN_EQUAL( mitc1->get_references(), 2 );


	// Copying ownership
	MyCopyableTestClass mctc1;
	BOOST_CHECK_PREDICATE( std::not_equal_to<std::size_t>(), ( (i->*"address_of")(           mctc1 ).to<std::size_t>() )( mctc1.get_address() ) );
	BOOST_CHECK_PREDICATE( std::equal_to<std::size_t>()    , ( (i->*"address_of")(boost::ref(mctc1)).to<std::size_t>() )( mctc1.get_address() ) );


	// -- Postconditions --
	rb_eval_string( "GC.start" );
	mtc1->inc = 42;
	BOOST_WARN_EQUAL( 1, mitc1->get_references() );
}

BOOST_AUTO_TEST_SUITE_END() // ruby_tests

#endif
