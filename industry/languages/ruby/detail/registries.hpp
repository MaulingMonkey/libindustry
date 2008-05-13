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
#include <map>

namespace industry { namespace languages { namespace ruby {
	template<class T, class B>
	struct class_;

	namespace detail {
		template<class T>
		struct class_registry {
			static VALUE get() {
				return intern();
			}

			static void set(VALUE v) {
				intern(v);
			}
		private:
			static VALUE intern(VALUE v = Qnil) {
				static VALUE result;
				if(v != Qnil)
					result = v;
				return result;
			}
		};

		template<> struct class_registry<void> { static VALUE get() { return rb_cObject; } };

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

		template<class T, class Fn>
		struct method_registry {
			static boost::function<Fn> const& get(std::string const& name, boost::function<Fn> f = boost::function<Fn>()) {
				static std::map<std::string, boost::function<Fn> > methods;
				if(!f.empty())
					methods[name] = f;
				return methods[name];
			}

			static VALUE call_proxy(VALUE self ...) {
				va_list args;
				va_start(args, self);
				return do_call_proxy<T, Fn, boost::function<Fn>::arity>::do_call_o(get(detail::ruby_value<std::string>::from(rb_eval_string("caller[0][/`([^']*)'/, 1]"))), self, args);
			}
		};

		template<class C, class T>
		struct variable_registry {
			static T C::* reg(std::string const& name, T C::* addr = 0) {
				static std::map<std::string, T C::*> vars;
				if(addr) {
					vars[name] = addr;
					vars[name + "="] = addr;
				}
				return vars[name];
			}

			static VALUE get(VALUE self) {
				return ruby_value<T>::to(ruby_value<C*>::from(self)->*reg(detail::ruby_value<std::string>::from(rb_eval_string("caller[0][/`([^']*)'/, 1]"))));
			}

			static VALUE set(VALUE self, VALUE value) {
				(ruby_value<C*>::from(self)->*reg(detail::ruby_value<std::string>::from(rb_eval_string("caller[0][/`([^']*)'/, 1]")))) = ruby_value<T>::from(value);
				return Qnil;
			}
		};
	}
}}}
#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
