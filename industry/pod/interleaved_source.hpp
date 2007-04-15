// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_POD_INTERLEAVED_SOURCE
#define IG_INDUSTRY_POD_INTERLEAVED_SOURCE

#ifndef INDUSTRY_POD_INTERLEAVED_LIMIT
#define INDUSTRY_POD_INTERLEAVED_LIMIT 10
#endif//INDUSTRY_POD_INTERLEAVED_LIMIT

#include <industry/nil.hpp>
#include <industry/traits/pod.hpp>
#include <boost/preprocessor.hpp>

#define DO_ORIGINAL_TYPEDEF(z,n,t) typedef BOOST_PP_CAT(t,n) BOOST_PP_CAT(BOOST_PP_CAT(original,n),_type);
#define DO_ADAPTED_TYPEDEF(z,n,t)  typedef typename to_pod_conversion_traits< BOOST_PP_CAT(t,n) >::pod_type \
                                   BOOST_PP_CAT(BOOST_PP_CAT(element,n),_type); /* FIXME:  Actually adapt the type */
#define DO_ADAPTED_ELEMENT(z,n,t)  BOOST_PP_CAT(BOOST_PP_CAT(element,n),_type)  BOOST_PP_CAT(element,n);


namespace industry {
	namespace pod {
		template < BOOST_PP_ENUM_BINARY_PARAMS( INDUSTRY_POD_INTERLEAVED_LIMIT , typename T , = nil BOOST_PP_INTERCEPT ) >
		struct interleaved_source {
			BOOST_PP_REPEAT( INDUSTRY_POD_INTERLEAVED_LIMIT , DO_ORIGINAL_TYPEDEF , T )
			BOOST_PP_REPEAT( INDUSTRY_POD_INTERLEAVED_LIMIT , DO_ADAPTED_TYPEDEF  , T )
			BOOST_PP_REPEAT( INDUSTRY_POD_INTERLEAVED_LIMIT , DO_ADAPTED_ELEMENT  , T )
		};
	}
}


#endif //ndef IG_INDUSTRY_POD_INTERLEAVED_SOURCE
