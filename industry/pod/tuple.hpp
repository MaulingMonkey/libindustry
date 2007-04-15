// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_POD_TUPLE
#define IG_INDUSTRY_POD_TUPLE

#ifndef INDUSTRY_POD_TUPLE_LIMIT
#define INDUSTRY_POD_TUPLE_LIMIT 10
#endif//INDUSTRY_POD_TUPLE_LIMIT

#include <industry/nil.hpp>
#include <industry/traits/pod.hpp>
#include <boost/preprocessor.hpp>

#define DO_TUPLE_TYPEDEF(z,n,t)  typedef BOOST_PP_CAT(t,n) BOOST_PP_CAT(BOOST_PP_CAT(element,n),_type);
#define DO_TUPLE_ELEMENT(z,n,t)  BOOST_PP_CAT(t,n) BOOST_PP_CAT(element,n);

namespace industry {
	namespace pod {
		template < BOOST_PP_ENUM_BINARY_PARAMS( INDUSTRY_POD_TUPLE_LIMIT , typename T , = nil BOOST_PP_INTERCEPT ) >
		struct tuple {
			BOOST_PP_REPEAT( INDUSTRY_POD_TUPLE_LIMIT , DO_TUPLE_TYPEDEF , T )
			BOOST_PP_REPEAT( INDUSTRY_POD_TUPLE_LIMIT , DO_TUPLE_ELEMENT , T )
		};

#define DO_TUPLE_SPECIALIZATION(z,n,unused)                                                                       \
		template < BOOST_PP_ENUM_PARAMS( n , typename T ) >                                                       \
		struct tuple< BOOST_PP_ENUM_PARAMS(n,T)  BOOST_PP_COMMA_IF(n)                                             \
		              BOOST_PP_ENUM_PARAMS( BOOST_PP_SUB(INDUSTRY_POD_TUPLE_LIMIT,n) , nil BOOST_PP_INTERCEPT )   \
		            > {                                                                                           \
			BOOST_PP_REPEAT( n , DO_TUPLE_TYPEDEF , T )                                                           \
			BOOST_PP_REPEAT( n , DO_TUPLE_ELEMENT , T )                                                           \
		};                                                                                                        \
/*---------------------------------------------------------------------------------------------------------------*/

		BOOST_PP_REPEAT( BOOST_PP_SUB(INDUSTRY_POD_TUPLE_LIMIT,1) , DO_TUPLE_SPECIALIZATION , ~ )
	}
}

#undef DO_TUPLE_TYPEDEF
#undef DO_TUPLE_ELEM
#undef DO_TUPLE_SPECIALIZATION

#endif //ndef IG_INDUSTRY_POD_TUPLE
