// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS

#include <industry/traits/function_traits.hpp>
#include <industry/languages/ruby/detail/ruby_value.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <stdarg.h>
#include <industry/languages/ruby/detail/do_call.hpp>

namespace industry { namespace languages { namespace ruby {
	template<class T>
	struct class_;

	namespace detail {
		template<class T, class Fn, unsigned int Arity>
		struct do_call_proxy {
			static VALUE do_call_o(boost::function<Fn> const& f, VALUE self, va_list args) {
				return do_call<T,
					Fn,
					boost::function<Fn>::arity,
					boost::is_same<T,
					typename boost::remove_const<
					typename boost::remove_pointer<
					typename boost::function<Fn>::arg1_type
					>::type
					>::type
					>::value
				>::call(f, self, args);
			}
		};

		template<class T, class Fn>
		struct do_call_proxy<T, Fn, 0> {
			static VALUE do_call_o(boost::function<Fn> const& f, VALUE self, va_list args) {
				return do_call<T, Fn, boost::function<Fn>::arity, false>::call(f, self, args);
			}
		};

		template<class T, class Fn, unsigned int N>
		struct class_n {
			static boost::function<Fn> const& get(boost::function<Fn> f = boost::function<Fn>()) {
				static boost::function<Fn> func;
				if(!f.empty())
					func = f;
				return func;
			}

			static VALUE call_proxy(VALUE self ...) {
				va_list args;
				va_start(args, self);
				return do_call_proxy<T, Fn, boost::function<Fn>::arity>::do_call_o(get(), self, args);
			}

			template<class Fn2>
			class_n<T, typename industry::function_traits<Fn2>::signature, N+1> def(std::string const& name, Fn2 f) {
				class_n<T, typename industry::function_traits<Fn2>::signature, N+1>::get(f);
				rb_define_method(::industry::languages::ruby::class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC((class_n<T, typename industry::function_traits<Fn2>::signature, N+1>::call_proxy)), industry::function_traits<Fn2>::arity);
				return class_n<T, typename industry::function_traits<Fn2>::signature, N+1>();
			}

			template<class Type>
			class_n& const_(std::string const& name, Type value) {
				rb_define_const(class_<T>::get_class(), name.c_str(), detail::ruby_value<Type>::to(value));
				return *this;
			}

			template<class V>
			class_n<T, void(), N+1> var(std::string const& name, V T::* p) {
				var_n<T, V, N+1>::reg(p);
				rb_define_method(::industry::languages::ruby::class_<T>::get_class(), name.c_str(), RUBY_METHOD_FUNC((var_n<T, V, N+1>::get)), 0);
				rb_define_method(::industry::languages::ruby::class_<T>::get_class(), (name + "=").c_str(), RUBY_METHOD_FUNC((var_n<T, V, N+1>::set)), 1);
				return class_n<T, void(), N+1>();
			}
		};

		template<class C, class T, unsigned int N>
		struct var_n {
			static T C::* reg(T C::* addr = 0) {
				static T C::* p = addr;
				return p;
			}

			static VALUE get(VALUE self) {
				return ruby_value<T>::to(ruby_value<C*>::from(self)->*reg());
			}

			static VALUE set(VALUE self, VALUE value) {
				(ruby_value<C*>::from(self)->*reg()) = ruby_value<T>::from(value);
				return Qnil;
			}
		};
	}
}}}
#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
