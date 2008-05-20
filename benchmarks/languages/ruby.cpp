#include <industry/languages/ruby/ruby.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;
using namespace industry::languages::ruby;

namespace {
	int i2 = 0;
	std::vector<int> array_1_000_000(1000000);
}

struct foo : self_aware<foo> {
	int i;
	
	foo(): i(0) {}

	static void selfless_call() { i2 += 1; }

	void call() { i += 1; }
	void call8( int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8 ) {
		// FIXME:  8 seems to be the current maximum number of arguments?
		i += a1+a2+a3+a4+a5+a6+a7+a8;
	}
	void calla1( value array ) {
		std::vector<int> a(array.length());
		for ( unsigned i = 0 ; i < a.size() ; ++i ) a[i] = array[i].to<int>();
	}
	void calla2( value array ) {
		std::vector<int> a(array_iterator<int>::begin(array), array_iterator<int>::end(array));
	}

	void filln() {
		std::vector<int> a(1000000);
		std::generate_n(a.begin(), a.size(), f2);
	}
	void set_f( const boost::function<void()>& new_f ) { f = new_f; }
	void set_f2(const boost::function<int()>& new_f2 ) { f2 = new_f2; }
	void call_f_1_000_000_times() { for ( int i = 0 ; i < 1000000 ; ++i ) f(); }

	boost::function<void()> f;
	boost::function<int()> f2;

	value make_array_1_000_000() {
		return value( array_1_000_000.begin(), array_1_000_000.end() );
	}
};

INDUSTRY_RUBY_MODULE( ruby_benchmarking ) {
	class_<foo>("Foo").
		def( "selfless_call"         , &foo::selfless_call          ).
		def( "call"                  , &foo::call                   ).
		def( "call8"                 , &foo::call8                  ).
		def( "calla1"                , &foo::calla1                 ).
		def( "calla2"                , &foo::calla2                 ).

		def( "f="                    , &foo::set_f                  ).
		def( "call_f_1_000_000_times", &foo::call_f_1_000_000_times ).
		def( "f2="                   , &foo::set_f2                 ).
		def( "filln"                 , &foo::filln                  ).
		
		def( "make_array_1_000_000"  , &foo::make_array_1_000_000   );

	eval("\
		class Foo\n\
			def mul2(array)\n\
				array.collect! {|i| i*2}\n\
			end\n\
		end\n\
	");
}

void benchmark_report( const char* description, double time ) {
	cout << format("%|-60| @ %|.3| second(s)") % description % time << endl;
}

void benchmark_eval( const char* expr ) {
	timer t;
	eval(expr);
	const double e = t.elapsed();
	benchmark_report(expr,e);
}

void print_eval( const char* expr ) {
	value v = eval(expr);
	string r = lexical_cast<string>(v);
	if ( r.size() > 37 ) {
		r.erase(38-3);
		r+="...";
	}
	cout << format("%|-37| => %|-37.37|") % expr % r << endl;
}

void benchmark_ruby() {
	using namespace boost;
	using namespace std;

	for ( unsigned i = 0 ; i < array_1_000_000.size() ; ++i ) array_1_000_000[i] = i;

	cout << "Ruby Benchmarks\n"
	     << "===============\n"
		 ;

	timer t;
	Init_ruby_benchmarking();
	const double init_time = t.elapsed();
	benchmark_report("Initializing the ruby_benchmarking module", init_time );
	cout << "\n";

	print_eval("$array1_000     = (0...1_000    ).to_a");
	print_eval("$array1_000_000 = (0...1_000_000).to_a");
	print_eval("$foo            = Foo.new");
	print_eval("$foo.f          = lambda{$i+=1}");
	print_eval("$foo.f2         = Proc.new{$q += 1}");
	print_eval("$q              = 0");
	print_eval("$i              = 0");
	cout << "\n";

	benchmark_eval  ("10_000.times{$foo.selfless_call}");
	cout << "  # static void foo::selfless_call() { i2 += 1; }\n";
	benchmark_eval  ("10_000.times{$foo.call}");
	cout << "  # void foo::call() { i += 1; }\n";
	benchmark_eval  ("10_000.times{$foo.call8(1,2,3,4,5,6,7,8)}");
	cout << "  # void foo::call8( int a1...a8 ) { i += a1...a8; }\n";
	benchmark_eval  ("10_000.times{$foo.calla1($array1_000)}");
	benchmark_eval  ("   100.times{$foo.calla1($array1_000_000)}");
	cout << "  # void foo::calla1( value array ) { for(...) vector[i]=array[i].to<int>(); }\n";
	benchmark_eval  ("10_000.times{$foo.calla2($array1_000)}");
	benchmark_eval  ("   100.times{$foo.calla2($array1_000_000)}");
	cout << "  # void foo::calla2( value array ) { vector(array_iterator<int>::begin,end); }\n";
	benchmark_eval  ("$foo.filln");
	cout << "  # void foo::filln() { std::generate(vector.begin(), 1000000, f2); }\n";
	benchmark_eval  ("$foo.call_f_1_000_000_times");
	cout << "  # void foo::call_f_1_000_000_times() { for(int i=0; i<1000000; ++i ) f(); }\n";
	benchmark_eval  ("    10.times{$foo.make_array_1_000_000}");
	cout << "  # value foo::make_array_1_000_000() { return value( begin, end ); }\n";
	cout << "\n";

	print_eval("$foo");
	print_eval("$i");
	cout << "\n";
}
