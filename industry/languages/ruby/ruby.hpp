// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_RUBY
#define IG_INDUSTRY_LANGUAGES_RUBY_RUBY

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4312 ) // value -> pointer size warnings
#pragma warning( disable: 4267 ) // pointer -> value size warnings
#endif

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <industry/languages/ruby/detail/ruby_value.hpp>
#include <industry/languages/ruby/detail/registries.hpp>
#include <industry/languages/ruby/call_f.hpp>
#include <industry/languages/ruby/declarations.hpp>
#include <industry/languages/ruby/eval.hpp>
#include <industry/languages/ruby/module.hpp>
#include <industry/languages/ruby/value.hpp>
#include <industry/config.hpp>
#include <industry/traits/function_traits.hpp>
#include <iostream>
#include <string>

namespace industry { namespace languages { namespace ruby {

#ifdef INDUSTRY_OS_LINUX
#	define INDUSTRY_RUBY_MODULE(name) void Do_##name##_Init(); \
	extern "C" void INDUSTRY_EXPORT Init_##name () {\
		static bool initialized = false;\
		if (initialized) {\
			std::clog << "WARNING:  " #name " not reloaded, linux multi-init seems to be bugged?" << std::endl;\
			return;\
		}\
		initialized = true;\
		ruby_init();\
		Do_##name##_Init();\
	}\
	void Do_##name##_Init()

#else // ndef INDUSTRY_OS_LINUX
#	define INDUSTRY_RUBY_MODULE(name) void Do_##name##_Init();\
	extern "C" void  INDUSTRY_EXPORT Init_##name () {\
		ruby_init();\
		Do_##name##_Init();\
	}\
	void Do_##name##_Init()

#endif //ndef INDUSTRY_OS_LINUX

	template<class Sig>
	struct init {
		template<class T>
		static void reg() {
			detail::constructor_registry<T, industry::function_traits<Sig>::arity>::template reg<Sig>();
		}
	};

	template<class T, class B>
	struct class_ {
		static void free_type(T* ptr) {
			delete ptr;
		}

		static VALUE alloc_type(VALUE klass) {
			T* ptr = static_cast<T*>(::operator new(sizeof(T)));
			return Data_Wrap_Struct(klass, 0, free_type, ptr);
		}

		static VALUE clone_type( const T& original ) {
			T* ptr = new T(original);
			return Data_Wrap_Struct(detail::class_registry<T>::get(), 0, free_type, ptr);
		}

		class_( const module& module, const std::string& name ) {
			detail::class_registry<T>::set((rb_define_class_under( module.get_value(), name.c_str(), rb_cObject )));
			rb_define_alloc_func(detail::class_registry<T>::get(), alloc_type);
			def(init<void()>());
		}

		class_(std::string const& name) {
			detail::class_registry<T>::set(rb_define_class(name.c_str(), detail::class_registry<B>::get()));
			rb_define_alloc_func(detail::class_registry<T>::get(), alloc_type);
			def(init<void()>());
		}

		template<class Sig>
		class_(const module& module, const std::string& name, init<Sig>) {
			detail::class_registry<T>::set((rb_define_class_under( module.get_value(), name.c_str(), rb_cObject )));
			rb_define_alloc_func(detail::class_registry<T>::get(), alloc_type);
			def(init<Sig>());
		}

		template<class Sig>
		class_(std::string const& name, init<Sig>) {
			detail::class_registry<T>::set(rb_define_class(name.c_str(), detail::class_registry<B>::get()));
			rb_define_alloc_func(detail::class_registry<T>::get(), alloc_type);
			def(init<Sig>());
		}

		class_( VALUE existing_class ) {
			detail::class_registry<T>::set(existing_class);
		}

		template<class Sig>
		class_& def(init<Sig>) {
			init<Sig>::template reg<T>();
			return *this;
		}

		template<class Fn2>
		class_& def(std::string const& name, Fn2 f) {
			typedef detail::method_registry<T, typename industry::function_traits<Fn2>::signature> f_registry;
			f_registry::get(name, f);
			rb_define_method(detail::class_registry<T>::get(), name.c_str(), RUBY_METHOD_FUNC(f_registry::call_proxy), industry::function_traits<Fn2>::arity);
			return *this;
		}

		template<class Fn2>
		class_& singleton_def(std::string const& name, Fn2 f) {
			typedef detail::method_registry<T, typename industry::function_traits<Fn2>::signature> f_registry;
			f_registry::get(name, f);
			rb_define_singleton_method(detail::class_registry<T>::get(), name.c_str(), RUBY_METHOD_FUNC(f_registry::call_proxy), industry::function_traits<Fn2>::arity);
			return *this;
		}

		template<class Type>
		class_& const_(std::string const& name, Type value) {
			static std::map<std::string, bool> constants;
			if(constants.find(name) == constants.end()) {
				rb_define_const(detail::class_registry<T>::get(), name.c_str(), detail::ruby_value<Type>::to(value));
				constants[name] = true;
			}
			return *this;
		}

		template<class V>
		class_& var(std::string const& name, V T::* p) {
			detail::variable_registry<T, V>::reg(name, p);
			rb_define_method(detail::class_registry<T>::get(), name.c_str(), RUBY_METHOD_FUNC((detail::variable_registry<T, V>::get)), 0);
			rb_define_method(detail::class_registry<T>::get(), (name + "=").c_str(), RUBY_METHOD_FUNC((detail::variable_registry<T, V>::set)), 1);
			return *this;
		}
	};	
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_INDUSTRY_LANGUAGES_RUBY_RUBY
