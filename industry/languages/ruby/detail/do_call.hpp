// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#if !BOOST_PP_IS_ITERATING
#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_DO_CALL
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_DO_CALL

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/for_each_product.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/type_traits.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 ) // unreferenced formal parameters
#endif

namespace industry { namespace languages { namespace ruby {

	template<class T, class B>
	struct class_;

	namespace detail {
		template<class T, class Fn, unsigned int Params, bool IsMemberFunction, bool IsVoid>
		struct do_call_is_void {
			typedef boost::function<Fn> func_type;
		};

#		define BOOST_PP_ITERATION_LIMITS (0, 17)
#		define BOOST_PP_FILENAME_1       <industry/languages/ruby/detail/do_call.hpp>
#		include BOOST_PP_ITERATE()

		template<class T, class Fn, unsigned int Params, bool IsMemberFunction>
		struct do_call {
			typedef boost::function<Fn> func_type;

			static VALUE call(func_type const& f, VALUE self, va_list args) {
				return do_call_is_void<T, Fn, Params, IsMemberFunction, boost::is_same<void, typename func_type::result_type>::value>::call(f, self, args);
			}
		};
	}
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_DO_CALL
#else //BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()
#define FUNCTION_ARG_TYPE(i) BOOST_PP_CAT(typename func_type::arg, BOOST_PP_CAT(BOOST_PP_INC(i),_type))
#define DO_WRAP_VALUE(x,i,d) (ruby_value<FUNCTION_ARG_TYPE(i)>::from(*(((VALUE*)args) + i)))
#define DO_WRAP_VALUE_MEMBER(z,i,data) (ruby_value<FUNCTION_ARG_TYPE(BOOST_PP_INC(i))>::from(*(((VALUE*)args) + i)))

template<class T, class Fn>
struct do_call_is_void<T, Fn, n, false, false> {
	typedef boost::function<Fn> func_type;
	static VALUE call(func_type const& f, VALUE self, va_list args) {
		return ruby_value<typename func_type::result_type>::to(f(BOOST_PP_ENUM(n, DO_WRAP_VALUE, BOOST_PP_EMPTY)));
	}
};

template<class T, class Fn>
struct do_call_is_void<T, Fn, n, true, false> {
	typedef boost::function<Fn> func_type;

	static VALUE call(func_type const& f, VALUE self, va_list args) {
		return ruby_value<typename func_type::result_type>::to(f(ruby_value<T*>::from(self) BOOST_PP_ENUM_TRAILING(BOOST_PP_DEC(n), DO_WRAP_VALUE_MEMBER, BOOST_PP_EMPTY)));
	}
};

template<class T, class Fn>
struct do_call_is_void<T, Fn, n, false, true> {
	typedef boost::function<Fn> func_type;
	static VALUE call(func_type const& f, VALUE self, va_list args) {
		f(BOOST_PP_ENUM(n, DO_WRAP_VALUE, BOOST_PP_EMPTY));
		return Qnil;
	}
};

template<class T, class Fn>
struct do_call_is_void<T, Fn, n, true, true> {
	typedef boost::function<Fn> func_type;

	static VALUE call(func_type const& f, VALUE self, va_list args) {
		f(ruby_value<T*>::from(self) BOOST_PP_ENUM_TRAILING(BOOST_PP_DEC(n), DO_WRAP_VALUE_MEMBER, BOOST_PP_EMPTY));
		return Qnil;
	}
};

#undef n
#undef DO_WRAP_VALUE
#undef DO_WRAP_VALUE_MEMBER
#undef FUNCTION_ARG_TYPE

#endif
