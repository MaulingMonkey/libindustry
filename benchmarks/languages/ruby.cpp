#include <industry/languages/ruby/ruby.hpp>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <iostream>

using namespace boost;
using namespace std;
using namespace industry::languages::ruby;

struct foo : self_aware<foo> {
	int i;
	
	foo(): i(0) {}

	void call() { i += 1; }
	void set_f( const boost::function<void()>& new_f ) { f = new_f; }
	void call_f_1_000_000_times() { for ( int i = 0 ; i < 1000000 ; ++i ) f(); }

	boost::function<void()> f;
};

INDUSTRY_RUBY_MODULE( ruby_benchmarking ) {
	class_<foo>("Foo").
		def( "call"                  , &foo::call                   ).
		def( "f="                    , &foo::set_f                  ).
		def( "call_f_1_000_000_times", &foo::call_f_1_000_000_times );
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

void benchmark_ruby() {
	using namespace boost;
	using namespace std;

#ifdef _DEBUG
	cout << "Ruby Benchmarks (DEBUG MODE)\n"
	     << "============================\n"
		 ;
#else
	cout << "Ruby Benchmarks (Release Mode)\n"
	     << "==============================\n"
		 ;
#endif

	timer t;
	Init_ruby_benchmarking();
	const double init_time = t.elapsed();
	benchmark_report("Initializing the ruby_benchmarking module", init_time );
	benchmark_eval  ("f=Foo.new; 10_000.times{f.call}");
	benchmark_eval  ("i=0; f=Foo.new; f.f=lambda{i+=1}; f.call_f_1_000_000_times");
}
