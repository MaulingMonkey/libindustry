// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_TRAITS_POD
#define IG_INDUSTRY_TRAITS_POD

#include <industry/sfinae.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace industry {
	namespace pod {
		namespace detail {
			template < typename T > sfinae::one has_pod_typedef_helper( typename T::pod_type* );
			template < typename T > sfinae::two has_pod_typedef_helper( ... );
			template < typename T > struct has_pod_typedef {
				enum { value = ( sizeof(sfinae::one) == sizeof(has_pod_typedef_helper<T>(0)) ) };
			};
			template < typename T > struct get_pod_typedef {
				typedef typename T::pod_type type;
			};
		}

		template < typename T                > struct   to_pod_conversion_traits {
			// Default case:  Hope for the best (that T is already POD or typedefs T::pod_type & provides cast operator)
			typedef typename boost::mpl::eval_if< detail::has_pod_typedef<T> , detail::get_pod_typedef<T> , boost::mpl::identity<T> >::type pod_type;

			static pod_type convert( const T & convertee ) { return convertee; }
		};

		template < typename T , typename POD > struct from_pod_conversion_traits {
			// Default case:  Hope for the best (that T has a constructor for from POD)

			static T convert( const POD & pod ) { return T(pod); }
		};
	}
}

#endif //ndef IG_INDUSTRY_TRAITS_POD
