// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 31, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_TEMPLATE_CRTP
#define IG_INDUSTRY_TEMPLATE_CRTP

#include <industry/inherit.hpp>
#include <industry/nil.hpp>
#include <boost/preprocessor.hpp>
#include <cassert>

namespace industry {
	namespace templates {
		template < typename SelfType = ::industry::nil >
		class crtp_self {
		protected:
			typedef SelfType self_type;
			self_type & self() {
				self_type * ptr = static_cast< self_type * >( this );
				return *ptr;
			}
			const self_type & self() const {
				const self_type * ptr = static_cast< const self_type * >( this );
				return *ptr;
			}
		};
		
		template <> class crtp_self< ::industry::nil > {
		public:
			public: template < typename Self > struct with_self { typedef crtp_self< Self > type; };
		};
		
		template < template < typename > class class_ > class crtp {
			public: template < typename Self > struct with_self { typedef class_< Self > type; };
		};
		
		#define IMPLEMENT_CRTPN(z,n,unused)                                                                                                                                     \
		template < template < typename , BOOST_PP_ENUM_PARAMS( n , typename BOOST_PP_INTERCEPT ) > class class_ , BOOST_PP_ENUM_PARAMS( n , typename T ) > class crtp ## n {    \
			public: template < typename Self > struct with_self { typedef class_< Self , BOOST_PP_ENUM_PARAMS( n , T ) > type; };                                                       \
		}; /*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		BOOST_PP_REPEAT_FROM_TO(1,10,IMPLEMENT_CRTPN,~)
		#undef IMPLEMENT_CRTPN
		
		namespace detail {
			template < typename CRTP , typename Self > struct crtp_apply_self { typedef typename CRTP::template with_self< Self >::type type; };
			template < typename Self > struct crtp_apply_self< ::industry::nil , Self > { typedef ::industry::nil type; };
		}
		
		#define INHERIT_ARGN(z,n,unused) typename ::industry::templates::detail::crtp_apply_self< CRTP ## n , Self >::type
		template < typename Self , BOOST_PP_ENUM_BINARY_PARAMS( INDUSTRY_INHERIT_LIMIT , typename CRTP , = ::industry::nil  BOOST_PP_INTERCEPT ) >
		class crtp_inherit : public ::industry::inherit< BOOST_PP_ENUM( INDUSTRY_INHERIT_LIMIT , INHERIT_ARGN , ~ ) >
		{};
		#undef INHERIT_ARGN
	}
}

#endif //ndef IG_INDUSTRY_TEMPLATE_CRTP
