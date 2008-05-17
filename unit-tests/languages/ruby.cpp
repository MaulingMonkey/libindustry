// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/foreach.hpp>
#include <boost/signal.hpp>
#include <industry/languages/ruby/ruby.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include <vector>

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

		void test() { name = eval<std::string>("caller[0][/`([^']*)'/, 1]"); }
		virtual int inherited() { return 1; }

		std::size_t address() const { return std::size_t(this); }

		virtual ~MyTestClass() {}
	};

	struct MyDerivedTestClass : MyTestClass {
	};

	struct MyOtherDerivedTestClass : MyTestClass {
		int inherited() { return 2; }
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

	class MyTestClassWithCallbacks {
	public:
		boost::function<float(float,float)> f_ff;

		void avoid_leak_reports() { f_ff.clear(); }
		float call_f_ff( float a, float b ) { return f_ff(a,b); }
		void set_f_ff_by_value(       boost::function<float(float,float)>  new_f_ff ) { f_ff = new_f_ff; }
		void set_f_ff_by_cref ( const boost::function<float(float,float)>& new_f_ff ) { f_ff = new_f_ff; }
	};

	class MyConstructorTest {
	public:
		MyConstructorTest(int x,int y) {
			test_value = x + y * y;
		}
		MyConstructorTest(int x) {
			test_value = x + x * x;
		}
		MyConstructorTest() {
			test_value = 5;
		}
		MyConstructorTest(int x, float y) {
			test_value = static_cast<int>(y * x);
		}
		MyConstructorTest( const std::string& s ) {
			// TODO:  Something?
		}
	};

	class MySelfAwareTest : public self_aware<MySelfAwareTest> {
	};

	void work1() {
		test_value += 1;
	}

	INDUSTRY_RUBY_MODULE(MyTestModule) {
		class_<MyTestClass> testClass = class_<MyTestClass>("MyTestClass").
			var("inc2", &MyTestClass::inc).
			def("work1", work1).
			def("work2", &MyTestClass::work2).
			var("inc", &MyTestClass::inc).
			def("address", &MyTestClass::address);
		testClass.def("mul_by_inc", &MyTestClass::mul_by_inc).
			const_("Multiplicand", 4).
			def("arr", &MyTestClass::arr).
			def("multi_arg_mul", &MyTestClass::multi_arg_mul).
			def("inherited", &MyTestClass::inherited).
			def("cal", &MyTestClass::test);

		class_<MyIntrusiveTestClass>("MyIntrusiveTestClass").
			def("references", &MyIntrusiveTestClass::get_references);

		class_<MyCopyableTestClass>("MyCopyableTestClass").
			def("address", &MyCopyableTestClass::get_address);

		class_<MyOtherDerivedTestClass, MyTestClass>("MyDerivedTestClass");

		class_<MyTestClassWithCallbacks>("MyTestClassWithCallbacks")
			.def( "avoid_leak_reports", &MyTestClassWithCallbacks::avoid_leak_reports )
			.def( "call_f_ff"         , &MyTestClassWithCallbacks::call_f_ff          )
			.def( "set_f_ff_by_value" , &MyTestClassWithCallbacks::set_f_ff_by_value  )
			.def( "set_f_ff_by_cref"  , &MyTestClassWithCallbacks::set_f_ff_by_cref   )
			;

		class_<MyConstructorTest>("MyConstructorTest", init<void(int,int)>()).
			def(init<void(int)>()).
			def(init<void()>()).
			def(init<void(int, float)>()).
			def(init<void(const std::string&)>());

		class_<MySelfAwareTest>("MySelfAwareTest" );
	}

	// prevent Boost.Test from detecting GCed objects as leaks:
	struct cleaner { ~cleaner() { rb_eval_string( "GC.start" ); } } instance;
}

BOOST_AUTO_TEST_SUITE( ruby_tests );

BOOST_AUTO_TEST_CASE( basic_invocation_test )
{
	test_value = 0;
	Init_MyTestModule();
	BOOST_CHECK_EQUAL( test_value, 0 );
	rb_eval_string("MyTestClass.new.work1" );
	BOOST_CHECK_EQUAL( test_value, 1 );
	rb_eval_string("MyTestClass.new.work2" );
	BOOST_CHECK_EQUAL( test_value, 3 );

	rb_eval_string( "GC.start" );


	rb_eval_string("MyTestClass.new.arr('hello')");
	BOOST_CHECK_EQUAL( name, "hello");
	rb_eval_string("MyTestClass.new.cal");
	BOOST_CHECK_EQUAL( name, "cal");
	rb_eval_string("MyConstructorTest.new(2, 3)");
	BOOST_CHECK_EQUAL( test_value, 11);
	rb_eval_string("MyConstructorTest.new(2)");
	BOOST_CHECK_EQUAL( test_value, 6);
	rb_eval_string("MyConstructorTest.new");
	BOOST_CHECK_EQUAL( test_value, 5);
	rb_eval_string("MyConstructorTest.new(2, 3.5)");
	BOOST_CHECK_EQUAL( test_value, 7);
	rb_eval_string("MyConstructorTest.new('sparta')");

	rb_eval_string( "GC.start" );
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

BOOST_AUTO_TEST_CASE( procs_and_blocks ) {
	Init_MyTestModule();
	boost::function< void (int) > f = call_f(eval( "Proc.new { |n| $a ||= 0; $a += n }" ));
	f(1);
	f(2);
	f(3);
	BOOST_CHECK_EQUAL( 6, eval<int>("$a") );

	boost::function< int (int,int) > add = call_f(eval( "Proc.new { |a,b| a+b }" ));
	BOOST_CHECK_EQUAL( add(1,2), 3 );
	BOOST_CHECK_EQUAL( add(2546,-1234), 1312 );

	boost::signal< int (int,int) > sig;
	sig.connect(call_f(eval("Proc.new { |a,b| $product     = a*b }")));
	sig.connect(call_f(eval("Proc.new { |a,b| $quotient    = a/b }")));
	sig.connect(call_f(eval("Proc.new { |a,b| $sum         = a+b }")));
	sig.connect(call_f(eval("Proc.new { |a,b| $difference  = a-b }")));
	BOOST_CHECK_EQUAL( sig(5,3), 2 );
	BOOST_CHECK_EQUAL( eval<int>("$product")   , 15 );
	BOOST_CHECK_EQUAL( eval<int>("$quotient")  ,  1 );
	BOOST_CHECK_EQUAL( eval<int>("$sum")       ,  8 );
	BOOST_CHECK_EQUAL( eval<int>("$difference"),  2 );

	eval("\
		mtcwc = MyTestClassWithCallbacks.new\n\
		mtcwc.set_f_ff_by_cref(  Proc.new { |a,b| a+b } )\n\
		$a = mtcwc.call_f_ff( 1.2345, 6.7890 )\n\
		mtcwc.set_f_ff_by_value( Proc.new { |a,b| a*b } )\n\
		$b = mtcwc.call_f_ff( 1.2345, 6.7890 )\n\
		mtcwc.avoid_leak_reports\n\
	");

	BOOST_CHECK_CLOSE( eval<float>("$a"), 8.0235f   , 0.01f );
	BOOST_CHECK_CLOSE( eval<float>("$b"), 8.3810205f, 0.01f );
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

	value safe_cast_test = eval("MyTestClass.new");
	BOOST_CHECK((value_static_cast<MyDerivedTestClass*, MyTestClass*>(safe_cast_test) != 0));
	BOOST_CHECK((value_dynamic_cast<MyDerivedTestClass*, MyTestClass*>(safe_cast_test) == 0));

	BOOST_CHECK_EQUAL((safe_cast_test->*"inherited")().to<int>(), 1);
	safe_cast_test = eval("4");

	value inheritance_test = eval("MyDerivedTestClass.new");
	BOOST_CHECK_EQUAL((inheritance_test->*"inherited")().to<int>(), 2);
	BOOST_CHECK((value_static_cast<MyOtherDerivedTestClass*, MyTestClass*>(inheritance_test) != 0));
	BOOST_CHECK((value_dynamic_cast<MyOtherDerivedTestClass*, MyTestClass*>(inheritance_test) != 0));

	value i = eval("\
		class VAE_Inspector\n\
			def pair3_ok( pair3 )   pair3  == [ -0x12345678, 0x87654321 ]  end\n\
			def tuple3_ok( tuple3 ) tuple3 == [ 0x12345678, 'sparta', 42 ] end\n\
		end\n\
		VAE_Inspector.new\
	");

	typedef std::pair<int,float> pair_if_t;
	pair_if_t pair1( 0x12345678, 3.14159265358979323f );
	pair_if_t pair2 = eval<pair_if_t>("[0x12345678,Math::PI]");
	BOOST_CHECK_EQUAL( pair1.first , pair2.first );
	BOOST_CHECK_CLOSE( pair1.second, pair2.second, 0.0001f );

	typedef std::pair<int,unsigned> pair_iu_t;
	pair_iu_t pair3( -0x12345678, 0x87654321 );
	BOOST_CHECK( (i->*"pair3_ok")(pair3).to<bool>() );

	typedef boost::tuple<short,int,float> tuple_sif_t;
	tuple_sif_t tuple1( 0x2468, 0x12345678, 3.14159265358979323f );
	tuple_sif_t tuple2 = eval<tuple_sif_t>("[0x2468,0x12345678,Math::PI]");
	BOOST_CHECK_EQUAL( tuple1.get<0>(), tuple2.get<0>() );
	BOOST_CHECK_EQUAL( tuple1.get<1>(), tuple2.get<1>() );
	BOOST_CHECK_CLOSE( tuple1.get<2>(), tuple2.get<2>(), 0.0001f );

	typedef boost::tuple<int,std::string,int> tuple_isi_t;
	tuple_isi_t tuple3( 0x12345678, "sparta", 42 );
	BOOST_CHECK( (i->*"tuple3_ok")(tuple3).to<bool>() );
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
			def check_equal( o1, o2 )\n\
				o1==o2\n\
			end\n\
			def identity(o)\n\
				o\n\
			end\n\
		end\n\
		");

	value i = eval("Inspector.new"); // give us something to (...->*"name")(...) from

	// Nontransfer of ownership
	std::auto_ptr<MyTestClass> mtc1( new MyTestClass() );
	BOOST_CHECK_EQUAL( "MyTestClass", (i->*"class_of")(mtc1.get()).to<std::string>() ); // raw pointer, shouldn't tranfser ownership
	BOOST_CHECK( (i->*"check_equal")(mtc1.get(), mtc1.get()).to<bool>() ); // we're sharing RDatas, right?
	BOOST_CHECK( mtc1.get() );

	
	// Transfer of ownership
	std::auto_ptr<MyTestClass> mtc2( new MyTestClass() );
	BOOST_CHECK_EQUAL( "MyTestClass", (i->*"class_of")(mtc2).to<std::string>() ); // directly passing auto_ptr, should transfer ownership
	BOOST_CHECK( !mtc2.get() );


	// Evil transfer of ownership
	std::auto_ptr<MyTestClass> mtc3( new MyTestClass() );
	MyTestClass* mtc4 = mtc3.get();
	BOOST_CHECK( (i->*"check_equal")(mtc3,mtc4).to<bool>() ); // we're sharing RDatas, right?


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


	// Self awareness
	value msat1 = eval( "MySelfAwareTest.new" );
	value msat2 = (i->*"identity")(msat1).to<MySelfAwareTest*>()->self();
	BOOST_CHECK_EQUAL( msat1, msat2 );


	// -- Postconditions --
	rb_eval_string( "GC.start" );
	mtc1->inc = 42;
	BOOST_WARN_EQUAL( 1, mitc1->get_references() );
}

BOOST_AUTO_TEST_CASE( ruby_array_awesomeness ) {
	std::vector<int> v_i;
	BOOST_FOREACH( int i , as_array_of<int>(eval("[1,2,3]")) ) v_i.push_back(i);

	BOOST_CHECK_EQUAL( v_i.size(), 3 );
	if ( v_i.size() >= 3 ) {
		BOOST_CHECK_EQUAL( v_i[0], 1 );
		BOOST_CHECK_EQUAL( v_i[1], 2 );
		BOOST_CHECK_EQUAL( v_i[2], 3 );
	}
}

BOOST_AUTO_TEST_SUITE_END(); // ruby_tests

#endif
