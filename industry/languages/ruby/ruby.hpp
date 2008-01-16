// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_LANGUAGES_RUBY_RUBY
#define IG_LANGUAGES_RUBY_RUBY

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4312 ) // value -> pointer size warnings
#pragma warning( disable: 4267 ) // pointer -> value size warnings
#endif

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <industry/languages/ruby/detail/ruby_value.hpp>
#include <industry/languages/ruby/detail/class.hpp>
#include <industry/languages/ruby/eval.hpp>
#include <industry/languages/ruby/value.hpp>
#include <industry/config.hpp>
#include <string>

namespace industry { namespace languages { namespace ruby {
#	define INDUSTRY_RUBY_MODULE(name) void Do_##name##_Init();\
	extern "C" void  INDUSTRY_EXPORT Init_##name () {\
		ruby_init();\
		Do_##name##_Init();\
	}\
	void Do_##name##_Init()

	template<class T>
	struct class_ {
		static VALUE get_class(VALUE v = 0) {
			static VALUE klass;
			if(v) {
				klass = v;
			}

			return klass;
		}

		static void free_type(T* ptr) {
			delete ptr;
		}

		static VALUE alloc_type(VALUE klass) {
			T* ptr = new T();
			return Data_Wrap_Struct(klass, 0, free_type, ptr);
		}

		class_(std::string const& name) {
			get_class(rb_define_class(name.c_str(), rb_cObject));
			rb_define_alloc_func(get_class(), alloc_type);
		}

		template<class Fn2>
		detail::class_n<T, typename industry::function_traits<Fn2>::signature, 0> def(std::string const& name, Fn2 f) {
			typedef detail::class_n<T, typename industry::function_traits<Fn2>::signature, 0> f_proxy_class;
			f_proxy_class::get(f);
			rb_define_method(class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC(f_proxy_class::call_proxy), industry::function_traits<Fn2>::arity);
			return f_proxy_class();
		}
	};
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_LANGUAGES_RUBY_RUBY
