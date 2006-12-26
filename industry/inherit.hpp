// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.inherit.hpp => industry/inherit.hpp
// May 20, 2006 - Created

#ifndef IG_INDUSTRY_INHERIT
#define IG_INDUSTRY_INHERIT

#include <industry/config.hpp>
#include <industry/nil.hpp>
#include <boost/preprocessor/repetition.hpp>

#ifndef INDUSTRY_INHERIT_LIMIT
#define INDUSTRY_INHERIT_LIMIT INDUSTRY_TEMPLATE_ARGUMENT_LIMIT
#endif //ndef INDUSTRY_INHERIT_LIMIT

namespace industry {
	namespace detail {
		template < unsigned , typename T > struct inherit_from : T {};
		template < unsigned N > struct inherit_from< N , nil > {};
		
		template < template < typename > class TT , unsigned , typename T > struct template_inherit_from : TT< T > {};
		template < template < typename > class TT , unsigned N > struct template_inherit_from< TT , N , nil > {};
		
		template < template < typename , typename > class TT , unsigned , typename Base , typename T > struct crtp_template_inherit_from : TT< Base , T > {};
		template < template < typename , typename > class TT , unsigned N , typename Base > struct crtp_template_inherit_from< TT , N , Base , nil > {};
	}
	
	#define MC_INDUSTRY_INHERIT_FROM_use(z, n, pre) detail::inherit_from< n , pre ## n >
	template < BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_INHERIT_LIMIT , typename T , nil ) >
	struct inherit : BOOST_PP_ENUM( INDUSTRY_INHERIT_LIMIT , MC_INDUSTRY_INHERIT_FROM_use , T ) {};
	#undef MC_INDUSTRY_INHERIT_FROM_use
	
	#define MC_INDUSTRY_TEMPLATE_INHERIT_FROM_use(z, n, data) \
		detail::template_inherit_from< BOOST_PP_TUPLE_ELEM(2,0,data) , n , BOOST_PP_CAT( BOOST_PP_TUPLE_ELEM(2,1,data) , n ) >
	template < template < typename > class TT , BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_INHERIT_LIMIT , typename T , nil ) >
	struct template_inherit : BOOST_PP_ENUM( INDUSTRY_INHERIT_LIMIT , MC_INDUSTRY_TEMPLATE_INHERIT_FROM_use , (TT , T) ) {};
	#undef MC_INDUSTRY_TEMPLATE_INHERIT_FROM_use
	
	#define MC_INDUSTRY_CRTP_TEMPLATE_INHERIT_FROM_use(z, n, data) detail::crtp_template_inherit_from \
		< BOOST_PP_TUPLE_ELEM(3,0,data) , n , BOOST_PP_TUPLE_ELEM(3,1,data) , BOOST_PP_CAT( BOOST_PP_TUPLE_ELEM(3,2,data) , n ) >
	template < template < typename , typename > class TT , typename Base , BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_INHERIT_LIMIT , typename T , nil ) >
	struct crtp_template_inherit : BOOST_PP_ENUM( INDUSTRY_INHERIT_LIMIT , MC_INDUSTRY_CRTP_TEMPLATE_INHERIT_FROM_use , (TT, Base, T) ) {};
	#undef MC_INDUSTRY_CRTP_TEMPLATE_INHERIT_FROM_use
}

#endif //ndef IG_INDUSTRY_INHERIT
