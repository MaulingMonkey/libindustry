// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_CALL_F
#define IG_INDUSTRY_LANGUAGES_RUBY_CALL_F

#include <industry/languages/ruby/value.hpp>

namespace industry { namespace languages { namespace ruby {
	class call_f {
		value proc;

		class return_proxy {
			lazy_value v;
		public:
			return_proxy( lazy_value v ): v(v) {}
			template < typename T > operator T() const { return v.to<T>(); }
		};
	public:
		call_f( value proc ): proc(proc) {}

/*------------------------------------------------------------------------------*/
#define BOOST_PP_LOCAL_MACRO(N)                                                  \
		template < BOOST_PP_ENUM_PARAMS(N,typename A) >                          \
		return_proxy operator()( BOOST_PP_ENUM_BINARY_PARAMS(N,A,arg) ) const {  \
			return return_proxy((proc->*"call")( BOOST_PP_ENUM_PARAMS(N,arg) )); \
		}                                                                        \
/*------------------------------------------------------------------------------*/
#define  BOOST_PP_LOCAL_LIMITS (1,10)
#include BOOST_PP_LOCAL_ITERATE()
#undef   BOOST_PP_LOCAL_LIMITS
#undef   BOOST_PP_LOCAL_MACRO
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_CALL_F
