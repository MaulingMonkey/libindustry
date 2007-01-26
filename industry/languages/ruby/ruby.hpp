// Copyright (c) 2006-2007 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
#if !BOOST_PP_IS_ITERATING

#ifndef IG_INDUSTRY_LANGUAGES_RUBY
#define IG_INDUSTRY_LANGUAGES_RUBY
#include <string>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/for_each_product.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <ruby.h>
#include <industry/traits/function_traits.hpp>

namespace industry { namespace languages { namespace ruby {
#define INDUSTRY_RUBY_MODULE(name) void Do_##name##_Init(); \
	extern "C" void __declspec(dllexport) Init_##name() { \
	ruby_init(); \
	Do_##name##_Init(); \
} \
	void Do_##name##_Init()

	namespace detail {
		template<class T>
		struct ruby_value { };

		template<class T> struct ruby_value<T*> { VALUE to_value(T* ptr) { return Data_Wrap_Struct(class_n<0, T>::get(), 0, RUBY_METHOD_FUNC(class_n_free_default), ptr); } T* from_value(VALUE v) { T* ptr; Data_Get_Struct(v, T, ptr); return ptr; } };

		template<> struct ruby_value<char> { VALUE to_value(char value) { return CHR2FIX(value); } char from_value(VALUE v) { return NUM2CHR(v); } };
		template<> struct ruby_value<short> { VALUE to_value(short value) { return INT2NUM(value); } short from_value(VALUE v) { return NUM2INT(v); } };
		template<> struct ruby_value<int> { VALUE to_value(int value) { return INT2NUM(value); } int from_value(VALUE v) { return NUM2INT(v); } };
		template<> struct ruby_value<long> { VALUE to_value(long value) { return LONG2NUM(value); } long from_value(VALUE v) { return NUM2INT(v); } };
		template<> struct ruby_value<unsigned char> { VALUE to_value(char value) { return UINT2NUM(value); } unsigned char from_value(VALUE v) { return NUM2UINT(v); } };
		template<> struct ruby_value<unsigned short> { VALUE to_value(short value) { return UINT2NUM(value); } unsigned short from_value(VALUE v) { return NUM2UINT(v); } };
		template<> struct ruby_value<unsigned int> { VALUE to_value(int value) { return UINT2NUM(value); } unsigned int from_value(VALUE v) { return NUM2UINT(v); } };
		template<> struct ruby_value<unsigned long> { VALUE to_value(long value) { return ULONG2NUM(value); } unsigned long from_value(VALUE v) { return NUM2ULONG(v); } };
		template<> struct ruby_value<char*> { VALUE to_value(char* value) { return rb_str_new2(value); } char* from_value(VALUE v) { return STR2CSTR(v); } };
		template<> struct ruby_value<const char*> { VALUE to_value(const char* value) { return rb_str_new2(value); } const char* from_value(VALUE v) { return STR2CSTR(v); } };
		template<> struct ruby_value<std::string> { VALUE to_value(std::string const& value) { return rb_str_new2(value.c_str()); } std::string from_value(VALUE v) { return std::string(STR2CSTR(v)); } };

		template<class Cl, class Fn, unsigned int N, unsigned int Ac>
		struct func_wrapper_helper {
			static VALUE dispatch(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv) {
				return Qnil;
			}
		};

#		define BOOST_PP_ITERATION_LIMITS (0, 25)
#		define BOOST_PP_FILENAME_1       "ruby.hpp"
#		include BOOST_PP_ITERATE()

		template<class Cl, class Fn, unsigned int N>
		struct func_wrapper : func_wrapper_helper<Cl, Fn, N, boost::function<Fn>::arity> {
			static boost::function<Fn> const& get_f(boost::function<Fn> f = boost::function<Fn>(0)) { static boost::function<Fn> fn; if(f) fn = boost::function<Fn>(f); return fn; }
			static VALUE func_call(int argc, VALUE *argv, VALUE self) {
				typedef boost::function<Fn> func_type;
				return dispatch(get_f(), self, argc, argv);
			}
		};

		template<unsigned int N, class T>
		struct class_n {
			class_n(VALUE klass) : klass(klass) {}

			template<class Fn>
			class_n<N+1, T> def(std::string const& name, Fn f) {
				typedef func_wrapper<T, typename function_traits<Fn>::signature, N> func_w;
				func_w::get_f(f);
				rb_define_method(klass, name.c_str(), RUBY_METHOD_FUNC(func_w::func_call), -1);
				return class_n<N+1, T>(klass);
			}

		private:
			VALUE klass;
		};

		template<class T>
		struct class_n<0, T> {
			class_n(VALUE klass) {
				get(klass);
				rb_define_singleton_method(get(), "new", RUBY_METHOD_FUNC(class_n_new_default), 0);
			}

			static VALUE class_n_new_default(VALUE klass) {
				T* ptr = new T();
				VALUE pdata = Data_Wrap_Struct(klass, 0, class_n_free_default, ptr);
				rb_obj_call_init(pdata, 0, 0);
				return pdata;
			}

			static void class_n_free_default(T* p) {
				delete p;
			}

			template<class Fn>
			class_n<1, T> def(std::string const& name, Fn* f) {
				typedef func_wrapper<T, Fn, 1> func_w;
				func_w::get_f(f);
				rb_define_method(get(), name.c_str(), RUBY_METHOD_FUNC(func_w::func_call), -1);
				return class_n<1, T>(get());
			}

			static VALUE get(VALUE v = VALUE()) { static VALUE klass; if(v) klass = v; return klass; }
		};
	}

	template<class T>
	detail::class_n<0, T> class_(std::string const& name) {
		return detail::class_n<0, T>(rb_define_class(name.c_str(), rb_cObject));
	}
}}}
#endif //IG_INDUSTRY_LANGUAGES_RUBY
#else

#define I BOOST_PP_ITERATION()
#define FUNCTION_ARG_TYPE(I) BOOST_PP_CAT(typename func_type::arg, BOOST_PP_CAT(BOOST_PP_INC(I),_type))
#define DO_WRAP_VALUE(z, n, data) ruby_value<FUNCTION_ARG_TYPE(n)>().from_value(argv[##n])
#define DO_WRAP_VALUE_2(z, n, data) ruby_value<FUNCTION_ARG_TYPE(BOOST_PP_INC(n))>().from_value(argv[n])
template<class Cl, class Fn, unsigned int N>
struct func_wrapper_helper<Cl, Fn, N, BOOST_PP_ITERATION()> {
	typedef boost::function<Fn> func_type;

	template<unsigned int C> struct dispatcher {
		template<typename T>
		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, T* p) {
			return dispatcher_util<boost::is_same<Cl, typename boost::remove_const<typename boost::remove_pointer<typename boost::remove_reference<typename func_type::arg1_type>::type>::type>::type>::value>::call(f, self, argc, argv, p);
		}
	};

	template<bool B> struct dispatcher_util {
		template<typename T>
		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, T* p) {
			if(argc < func_type::arity)
				rb_bug("Argument count mismatch. Expected %d found %d", func_type::arity, argc);
			return ruby_value<typename boost::function<Fn>::result_type>().to_value(f(BOOST_PP_ENUM(I, DO_WRAP_VALUE, BOOST_PP_EMPTY)));
		}

		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, void* p) {
			if(argc < func_type::arity)
				rb_bug("Argument count mismatch. Expected %d found %d", func_type::arity, argc);
			f(BOOST_PP_ENUM(I, DO_WRAP_VALUE, BOOST_PP_EMPTY));
			return Qnil;
		}
	};

	template<> struct dispatcher_util<true> {
		template<typename T>
		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, T* p) {
			if(argc < func_type::arity - 1)
				rb_bug("Argument count mismatch. Expected %d found %d", func_type::arity, argc);
			Cl* ptr;
			Data_Get_Struct(self, Cl, ptr);
			return ruby_value<typename boost::function<Fn>::result_type>().to_value(f(ptr, BOOST_PP_ENUM(BOOST_PP_DEC(I), DO_WRAP_VALUE_2, BOOST_PP_EMPTY)));
		}

		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, void* p) {
			if(argc < func_type::arity - 1)
				rb_bug("Argument count mismatch. Expected %d found %d", func_type::arity, argc);
			Cl* ptr;
			Data_Get_Struct(self, Cl, ptr);
			f(ptr, BOOST_PP_ENUM(BOOST_PP_DEC(I), DO_WRAP_VALUE_2, BOOST_PP_EMPTY));
			return Qnil;
		}
	};

	template<> struct dispatcher<0> {
		template<typename T>
		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, T* p) {
			return ruby_value<typename boost::function<Fn>::result_type>().to_value(f());
		}

		static VALUE call(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, void* p) {
			f();
			return Qnil;
		}
	};

	static VALUE dispatch(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv) {
		return dispatch(f, self, argc, argv, (typename boost::function<Fn>::result_type*)(0));
	}

	template<typename T>
	static VALUE dispatch(boost::function<Fn> const& f, VALUE self, int argc, VALUE* argv, T* p) {
		return dispatcher<func_type::arity>::call(f, self, argc, argv, p);
	}
};
#undef DO_WRAP_VALUE_2
#undef I
#undef FUNCTION_ARG_TYPE
#undef DO_WRAP_VALUE
#endif