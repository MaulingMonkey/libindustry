// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 ) // unreferenced formal parameters
#pragma warning( disable: 4345 )
#endif

#include <industry/traits/function_traits.hpp>
#include <industry/languages/ruby/detail/intrusive.hpp>
#include <industry/languages/ruby/detail/ruby_value.hpp>
#include <industry/languages/ruby/detail/constructor_registry.hpp>
#include <industry/languages/ruby/detail/self_aware_base.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <stdarg.h>
#include <industry/languages/ruby/detail/do_call.hpp>
#include <map>

namespace industry { namespace languages { namespace ruby {
	template<class T, class B>
	struct class_;

	namespace detail {
		extern std::map<void*,VALUE> instances;

		template<class T>
		struct instance_registry {
		private:
			static VALUE get_ruby_value_impl( void* ptr ) {
				VALUE& v = instances[ptr];
				if (!v) {
					intrusive_add_ref_or_noop::call((T*)ptr);
					v = Data_Wrap_Struct(detail::class_registry<T>::get(),0,intrusive_gc_and_release_or_noop::call<T>,ptr); // assume it's owned by C++
				}
				return v;
			}
			static VALUE get_ruby_value_impl( detail::self_aware_base<T>* ptr ) { return ptr->raw_self(); }

			static void ruby_gced_impl( void*                       ptr ) { instances.erase(ptr); }
			static void ruby_gced_impl( detail::self_aware_base<T>* ptr ) { ptr->v = 0; }

			static void ruby_initialized_impl( void* ptr                      , VALUE v ) { assert(!instances[ptr]); instances[ptr] = v; }
			static void ruby_initialized_impl( detail::self_aware_base<T>* ptr, VALUE v ) { ptr->v = v; }
		public:
			static VALUE register_ruby_owned(T* ptr) {
				VALUE v = get_ruby_value(ptr);
				RDATA(v)->dfree = RUBY_DATA_FUNC(intrusive_gc_and_release_or_delete::call<T>);
				return v;
			}
			static VALUE get_ruby_value(T* ptr) { return get_ruby_value_impl(ptr); }
			static void ruby_gced(T* ptr) { ruby_gced_impl(ptr); }
			static void ruby_initialized(T* ptr, VALUE v) { ruby_initialized_impl(ptr,v); }
		};

		std::map<VALUE,VALUE>& remap_registry();

		template<class T, unsigned int Arity>
		struct constructor_registry {
			template<class Sig>
			static void reg() {
				initialize();
				constructor_registry_impl<T, Sig, Arity>::reg();
			}

			static bool initialized(int p = 0) {
				static bool initted;
				if(p) {
					initted = true;
					constructor_registry<T, Arity - 1>::initialized(1);
				}

				return initted;
			}
			static void initialize() {
				if(!initialized()) {
					initialized(1);
					rb_define_method(detail::class_registry<T>::get(), "initialize", RUBY_METHOD_FUNC(construct), -1);
				}
			}

			static std::map<std::string, VALUE(*)(int, VALUE*, VALUE)>& get_constructors() {
				static std::map<std::string, VALUE(*)(int, VALUE*, VALUE)> constructors;
				return constructors;
			}

			static VALUE construct(int argc, VALUE* argv, VALUE self) {
				if(argc < Arity)
					return constructor_registry<T, Arity - 1>::construct(argc, argv, self);
				else if(argc != Arity)
					rb_raise(rb_eNoMethodError, "No such initializer defined.");

				std::string key;
				for(int i = 0; i < argc; ++i) {
					VALUE klass = CLASS_OF(argv[i]);
					if ( remap_registry().find(klass) != remap_registry().end() ) klass = remap_registry()[klass];
					key += rb_class2name(klass);
				}

				if(get_constructors().find(key) != get_constructors().end()) {
					return get_constructors()[key](argc, argv, self);
				} else {
					rb_raise(rb_eNoMethodError, "No such constructor defined.");
				}

				return self;
			}
		};

		template<class T>
		struct constructor_registry<T, 0> {
			template<class Sig>
			static void reg() {
				initialize();
				constructor_registry_impl<T, Sig, 0>::reg();
			}

			static bool initialized(int p = 0) {
				static bool initted;

				if(p) {
					initted = true;
				}

				return initted;
			}

			static std::map<std::string, VALUE(*)(int, VALUE*, VALUE)>& get_constructors() {
				static std::map<std::string, VALUE(*)(int, VALUE*, VALUE)> constructors;
				return constructors;
			}

			static void initialize() {
				if(!initialized()) {
					initialized(1);
					rb_define_method(detail::class_registry<T>::get(), "initialize", RUBY_METHOD_FUNC(construct), -1);
				}
			}

			static VALUE construct(int argc, VALUE* argv, VALUE self) {
				if(argc != 0)
					rb_raise(rb_eNoMethodError, "No such initializer defined.");

				get_constructors()[""](argc, argv, self);

				return self;
			}
		};

		template<class T>
		struct class_registry {
			static VALUE get() {
				assert(intern());
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
			static void reg_reader(std::string const& name, T C::* addr ) {
				vars(name) = addr;
			}
			static void reg_writer(std::string const& name, T C::* addr ) {
				vars(name + "=") = addr;
			}
			static void reg(std::string const& name, T C::* addr ) {
				vars(name) = addr;
				vars(name + "=") = addr;
			}

			static T C::*& vars( std::string const& name ) {
				static std::map<std::string, T C::*> storage;
				return storage[name];
			}

			static VALUE get(VALUE self) {
				return ruby_value<T>::to(ruby_value<C*>::from(self)->*vars(detail::ruby_value<std::string>::from(rb_eval_string("caller[0][/`([^']*)'/, 1]"))));
			}

			static VALUE set(VALUE self, VALUE value) {
				(ruby_value<C*>::from(self)->*vars(detail::ruby_value<std::string>::from(rb_eval_string("caller[0][/`([^']*)'/, 1]")))) = ruby_value<T>::from(value);
				return Qnil;
			}
		};
	}
}}}
#pragma warning(pop)
#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CLASS
