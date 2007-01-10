// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.multitype.hpp => industry/multitype.hpp
// May 27, 2006 - Changed from forwarding operator=s (caused multidef errors in VS2k5) to CRTP + using statements.
// May 20, 2006 - Created

#ifndef IG_INDUSTRY_MULTITYPE
#define IG_INDUSTRY_MULTITYPE

#include <industry/config.hpp>
#include <industry/nil.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <cassert>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4522 ) //multiple assignment operators warning
#endif //def _MSC_VER

#ifndef INDUSTRY_MULTITYPE_LIMIT
#define INDUSTRY_MULTITYPE_LIMIT INDUSTRY_TEMPLATE_ARGUMENT_LIMIT
#endif //ndef INDUSTRY_MULTITYPE_LIMIT

namespace industry {
	namespace detail {
		template < typename Base , unsigned N , typename T > struct multitype_aspect {
			T value;
		public:
			typedef const T & target_type; //used by multitype operator= generator.
			
			operator T & (void) { return value; }
			operator const T & (void) const { return value; }
			Base & operator=( target_type new_value ) {
				value = new_value;
				return *static_cast< Base * >( this );
			}
		};
		
		template < unsigned N > struct multitype_dummy_target {};
		template < typename Base , unsigned N > struct multitype_aspect< Base , N , nil > {
			//Dummy no-op operator= allows multitype to use or generate matching operator=s,
			//  without having different specializations for every missing one.
			typedef multitype_dummy_target< N > target_type;
			Base & operator=( target_type ) {
				assert( !"used" );
				return *static_cast< Base * >( this );
			}
		};
	}
	
	#define Tn               BOOST_PP_ENUM_PARAMS( INDUSTRY_MULTITYPE_LIMIT , T )
	#define TYPENAME_Tn      BOOST_PP_ENUM_PARAMS( INDUSTRY_MULTITYPE_LIMIT , typename T )
	#define TYPENAME_NIL_Tn  BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_MULTITYPE_LIMIT , typename T , nil )
	#define INDUSTRY_MULTITYPE_ASPECT_name(z,n,pre)            detail::multitype_aspect< multitype< Tn > , n , pre ## n >
	#define INDUSTRY_MULTITYPE_ASPECT_use_assigner(z, n, pre)  using INDUSTRY_MULTITYPE_ASPECT_name(z,n,pre) ::operator=;
		
	template < TYPENAME_NIL_Tn >
	struct multitype : BOOST_PP_ENUM( INDUSTRY_MULTITYPE_LIMIT , INDUSTRY_MULTITYPE_ASPECT_name , T ) {
		BOOST_PP_REPEAT( INDUSTRY_MULTITYPE_LIMIT , INDUSTRY_MULTITYPE_ASPECT_use_assigner , T )
	};
	#undef Tn
	#undef TYPENAME_Tn
	#undef TYPENAME_NIL_Tn
	#undef INDUSTRY_MULTITYPE_ASPECT_name
	#undef INDUSTRY_MULTITYPE_ASPECT_use_assigner
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif //def _MSC_VER

#endif //ndef IG_INDUSTRY_MULTITYPE
