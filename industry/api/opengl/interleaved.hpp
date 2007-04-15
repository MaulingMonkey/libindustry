// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_INTERLEAVED
#define IG_INDUSTRY_API_OPENGL_INTERLEAVED

#include <industry/nil.hpp>
#include <boost/array.hpp>
#include <boost/preprocessor.hpp>

#define INDUSTRY_API_OPENGL_INTERLEAVE_LIMIT 5

// Local macros:
#define TYPENAME_N_WITH_NIL_DEFAULT(z,n,T) typename BOOST_PP_CAT(T,n) = nil
#define DEFINE_MEMBER_ARRAY(z,n,T)         boost::array< typename BOOST_PP_CAT(T,n) :: component_type , BOOST_PP_CAT(T,n) :: components > BOOST_PP_CAT(component,n) ;
#define ECHO(z,n,data)                     data

/*---------------------------------------------------------------*/
#define DO_SPECIALIZATION(n,max)                                  \
	< BOOST_PP_ENUM_PARAMS(n,T)                                   \
	  BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB(INDUSTRY_API_OPENGL_INTERLEAVE_LIMIT,n) , ECHO , nil )  \
	>                                                             \
/*---------------------------------------------------------------*/
#define SPECIALIZATION(n,max) BOOST_PP_EXPR_IIF                   \
	( BOOST_PP_NOT_EQUAL(n,max)                                   \
	, DO_SPECIALIZATION(n,max)                                    \
	)                                                             \
/*---------------------------------------------------------------*/
#define ITERATION(z,n,max)                                        \
template < BOOST_PP_ENUM_PARAMS( n, typename T ) >                \
	struct interleaved SPECIALIZATION(n,max)                      \
{                                                                 \
	BOOST_PP_REPEAT( n , DEFINE_MEMBER_ARRAY , T )                \
};                                                                \
/*---------------------------------------------------------------*/

namespace industry {
	namespace api {
		namespace opengl {
			template < typename T0, BOOST_PP_ENUM_SHIFTED( INDUSTRY_API_OPENGL_INTERLEAVE_LIMIT , TYPENAME_N_WITH_NIL_DEFAULT , T ) > struct interleaved;

			BOOST_PP_REPEAT_FROM_TO( 1, INDUSTRY_API_OPENGL_INTERLEAVE_LIMIT , ITERATION , INDUSTRY_API_OPENGL_INTERLEAVE_LIMIT )
		}
	}
}

#undef TYPENAME_N_WITH_NIL_DEFAULT
#undef DEFINE_MEMBER_ARRAY
#undef ECHO
#undef SPECIALIZATION
#undef ITERATION

#endif //ndef IG_INDUSTRY_API_OPENGL_INTERLEAVED
