// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  1, 2007 - Created

#ifndef INDUSTRY_TEMPLATE_TT_LIMIT
#define INDUSTRY_TEMPLATE_TT_LIMIT 10
#endif //ndef INDUSTRY_TEMPLATE_TT_LIMIT

namespace industry {
	namespace templates {
		#define IMPL_TTn(z,n,unused)                                                                                       \
		template < template < BOOST_PP_ENUM_PARAMS( n , typename BOOST_PP_INTERCEPT ) > class class_ > struct tt ## n {    \
			template < BOOST_PP_ENUM_PARAMS( n , typename T ) > struct with {                                              \
				typedef class_< BOOST_PP_ENUM_PARAMS( n , T ) > type;                                                      \
			};                                                                                                             \
		}; /*-------------------------------------------------------------------------------------------------------------*/
		BOOST_PP_REPEAT_FROM_TO( 1 , BOOST_PP_ADD( 1 , INDUSTRY_TEMPLATE_TT_LIMIT ) , IMPL_TTn , ~ )
		#undef IMPL_TTn
	}
	using ::industry::templates::tt1;
}
