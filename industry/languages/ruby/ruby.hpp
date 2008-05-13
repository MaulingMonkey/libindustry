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
#include <industry/languages/ruby/detail/registries.hpp>
#include <industry/languages/ruby/eval.hpp>
#include <industry/languages/ruby/module.hpp>
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

		static VALUE clone_type( const T& original ) {
			T* ptr = new T(original);
			return Data_Wrap_Struct(get_class(), 0, free_type, ptr);
		}

		class_( const module& module, const std::string& name ) {
			get_class(rb_define_class_under( module.get_value(), name.c_str(), rb_cObject ));
			rb_define_alloc_func(get_class(), alloc_type);
		}

		class_(std::string const& name) {
			get_class(rb_define_class(name.c_str(), rb_cObject));
			rb_define_alloc_func(get_class(), alloc_type);
		}

		class_(std::string const& name, VALUE baseClass) {
			get_class(rb_define_class(name.c_str(), baseClass));
			rb_define_alloc_func(get_class(), alloc_type);
		}

		class_( VALUE existing_class ) {
			get_class(existing_class);
		}

		template<class Fn2>
		class_& def(std::string const& name, Fn2 f) {
			typedef detail::method_registry<T, typename industry::function_traits<Fn2>::signature, 0> f_proxy_class;
			f_proxy_class::get(name, f);
			rb_define_method(class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC(f_proxy_class::call_proxy), industry::function_traits<Fn2>::arity);
			return *this;
		}

		template<class Fn2>
		class_& singleton_def(std::string const& name, Fn2 f) {
			typedef detail::method_registry<T, typename industry::function_traits<Fn2>::signature, 0> f_proxy_class;
			f_proxy_class::get(name, f);
			rb_define_singleton_method(class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC(f_proxy_class::call_proxy), industry::function_traits<Fn2>::arity);
			return *this;
		}

		template<class Type>
		class_& const_(std::string const& name, Type value) {
			static std::map<std::string, bool> constants;
			if(constants.find(name) == constants.end()) {
				rb_define_const(class_<T>::get_class(), name.c_str(), detail::ruby_value<Type>::to(value));
				constants[name] = true;
			}
			return *this;
		}

		template<class V>
		class_& var(std::string const& name, V T::* p) {
			detail::variable_registry<T, V>::reg(name, p);
			rb_define_method(::industry::languages::ruby::class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC((detail::variable_registry<T, V>::get)), 0);
			rb_define_method(::industry::languages::ruby::class_<T>::get_class(), (name + "=").c_str(), RUBY_METHOD_FUNC((detail::variable_registry<T, V>::set)), 1);
			return *this;
		}
	};

	template<class T>
	VALUE base() {
		return class_<T>::get_class();
	}
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_LANGUAGES_RUBY_RUBY
