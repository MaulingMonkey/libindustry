// Copyright (c) 2006-2007 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
#ifndef IG_INDUSTRY_LANGUAGES_RUBY
#define IG_INDUSTRY_LANGUAGES_RUBY

#include <string>
#include <boost/function.hpp>
#include <ruby.h>


namespace industry { namespace languages { namespace ruby {
#define INDUSTRY_RUBY_MODULE(name) void Do_##name_Init(); \
	extern "C" void __declspec(dllexport) Init_##name() { \
		ruby_init(); \
		Do_##name_Init(); \
	} \
	void Do_##name_Init()

	namespace detail {
		template<class F>
		struct func_wrapper {
			static boost::function<F> get_f(F* f = 0) { static F* fn; if(f) fn = f; return boost::function<F>(fn); }
			static VALUE func_call(int argc, VALUE *argv, VALUE self) {
				get_f()();
				return Qnil;
			}
		};

		template<unsigned int N, class T>
		struct class_n {
			class_n(VALUE klass) : klass(klass) {}

			template<class Fn>
			class_n<N+1, T> def(std::string const& name, Fn* f) {
				typedef func_wrapper<Fn> func_w;
				func_w::get_f(f);
				rb_define_method(klass, name.c_str(), RUBY_METHOD_FUNC(func_w::func_call), -1);
				return class_n<N+1, T>(klass);
			}
		private:
			VALUE klass;
		};
	}

	template<class T>
	detail::class_n<0, T> class_(std::string const& name) {
		return detail::class_n<0, T>(rb_define_class(name.c_str(), rb_cObject));
	}
}}}
#endif //IG_INDUSTRY_LANGUAGES_RUBY