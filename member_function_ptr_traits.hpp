// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 28, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_MEMBER_FUNCTION_PTR_TRAITS
#define IG_INDUSTRY_MEMBER_FUNCTION_PTR_TRAITS

#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition.hpp>

namespace industry {
	template < typename F >
	struct member_function_ptr_traits;
		
	#define MEMBER_FUNCTION_PTR_TRAITS_TYPEDEF(z,n,id) \
		typedef A ## n    id ## n ## _type;
	
	#define MEMBER_FUNCTION_PTR_TRAITS(z,n,unused)                                          \
	template < typename C , typename R  BOOST_PP_ENUM_TRAILING_PARAMS( n , typename A ) >   \
	struct member_function_ptr_traits< R (C::*)( BOOST_PP_ENUM_PARAMS( n , A ) ) > {        \
		typedef R result_type;                                                              \
		typedef C class_type;                                                               \
		BOOST_PP_REPEAT( n , MEMBER_FUNCTION_PTR_TRAITS_TYPEDEF , arg );                    \
	}; /* -------------------------------------------------------------------------------- */
	
	#define MEMBER_FUNCTION_PTR_TRAITS_CONST(z,n,unused)                                    \
	template < typename C , typename R  BOOST_PP_ENUM_TRAILING_PARAMS( n , typename A ) >   \
	struct member_function_ptr_traits< R (C::*)( BOOST_PP_ENUM_PARAMS( n , A ) ) const > {  \
		typedef R result_type;                                                              \
		typedef const C class_type;                                                         \
		BOOST_PP_REPEAT( n , MEMBER_FUNCTION_PTR_TRAITS_TYPEDEF , arg );                    \
	}; /* -------------------------------------------------------------------------------- */
	
	BOOST_PP_REPEAT(10,MEMBER_FUNCTION_PTR_TRAITS,~);
	BOOST_PP_REPEAT(10,MEMBER_FUNCTION_PTR_TRAITS_CONST,~);
	
	//#undef MEMBER_FUNCTION_PTR_TRAITS_TYPEDEF
	//#undef MEMBER_FUNCTION_PTR_TRAITS
	//#undef MEMBER_FUNCTION_PTR_TRAITS_CONST
}

#endif //ndef IG_INDUSTRY_MEMBER_FUNCTION_PTR_TRAITS
