// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 31, 2006 - Created

#ifndef IG_INDUSTRY_TEMPLATE_GROUP
#define IG_INDUSTRY_TEMPLATE_GROUP

#include <industry/config.hpp>
#include <industry/nil.hpp>
#include <boost/preprocessor/repetition.hpp>

// Controls the maximum number of template arguments available per group (e.g. max N for template#_group< T1 , T2 .. TN >)
#ifndef INDUSTRY_TEMPLATE_GROUP_LIMIT
#define INDUSTRY_TEMPLATE_GROUP_LIMIT 10
#endif //ndef INDUSTRY_TEMPLATE_GROUP_LIMIT

// Controls the maximum number of template template arguments per template per group available (e.g. max N for templateN_group)
#ifndef INDUSTRY_TEMPLATE_GROUP_TT_LIMIT
#define INDUSTRY_TEMPLATE_GROUP_TT_LIMIT 10
#endif //ndef INDUSTRY_TEMPLATE_GROUP_TT_LIMIT


#define INDUSTRY_TEMPLATE_GROUP_TYPENAMES              BOOST_PP_ENUM_PARAMS( INDUSTRY_TEMPLATE_GROUP_LIMIT , typename Item )
#define INDUSTRY_TEMPLATE_TRAILING_GROUP_TYPENAMES     BOOST_PP_ENUM_TRAILING_PARAMS( INDUSTRY_TEMPLATE_GROUP_LIMIT , typename Item )

#define INDUSTRY_TEMPLATE_GROUPN_TYPENAMES(n)          BOOST_PP_ENUM_PARAMS( INDUSTRY_TEMPLATE_GROUP_LIMIT , template < BOOST_PP_ENUM_PARAMS( n , typename BOOST_PP_INTERCEPT ) > class Item )
#define INDUSTRY_TEMPLATE_TRAILING_GROUPN_TYPENAMES(n) BOOST_PP_ENUM_TRAILING_PARAMS( INDUSTRY_TEMPLATE_GROUP_LIMIT , template < BOOST_PP_ENUM_PARAMS( n , typename BOOST_PP_INTERCEPT ) > class Item )

#define INDUSTRY_TEMPLATE_GROUPN_TYPENAME_DECL(z,n,ttn)                                       \
	BOOST_PP_COMMA_IF(n)                                                                      \
	template < BOOST_PP_ENUM_PARAMS( ttn , typename   BOOST_PP_INTERCEPT ) > class Item ## n  \
		=  ::industry::template ## ttn ## _nil                                                \
/*-------------------------------------------------------------------------------------------*/
#define INDUSTRY_TEMPLATE_GROUPN_TYPENAMES_DECL(n)  BOOST_PP_REPEAT( INDUSTRY_TEMPLATE_GROUP_LIMIT , INDUSTRY_TEMPLATE_GROUPN_TYPENAME_DECL , n )
#define INDUSTRY_IMPLEMENT_TEMPLATEN_GROUP(z,n,unused) template < INDUSTRY_TEMPLATE_GROUPN_TYPENAMES_DECL(n) > struct tt ## n ## _group {};

namespace industry {
	namespace templates {
		template < BOOST_PP_ENUM_BINARY_PARAMS( INDUSTRY_TEMPLATE_GROUP_LIMIT , typename Item , = ::industry::nil  BOOST_PP_INTERCEPT ) > struct group {};
		BOOST_PP_REPEAT_FROM_TO( 1 , BOOST_PP_ADD( 1 , INDUSTRY_TEMPLATE_GROUP_TT_LIMIT ) , INDUSTRY_IMPLEMENT_TEMPLATEN_GROUP , ~ )
	}
	using ::industry::templates::group;
	#define USEN(z,n,unused) using ::industry::templates::tt ## n ## _group;
	BOOST_PP_REPEAT_FROM_TO( 1 , BOOST_PP_ADD( 1 , INDUSTRY_TEMPLATE_GROUP_TT_LIMIT ) , USEN , ~ )
	#undef USEN
}

#endif //ndef IG_INDUSTRY_TEMPLATE_GROUP
