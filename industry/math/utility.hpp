// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  4, 2007 - Created

#include <limits>

namespace industry {
	namespace math {
		namespace detail {
			template < bool is_signed , typename T > struct positive { static bool impl( const T & value ) { return value >= 0; } };
			template < typename T > struct positive< false , T >     { static bool impl( const T &       ) { return true; } };
		}
	
		template < typename T >
		inline bool is_positive( const T & value ) {
			return ::industry::math::detail::positive< std::numeric_limits<T>::is_signed , T >::impl( value );
		}
		
		template < typename T >
		inline bool is_negative( const T & value ) {
			return !is_positive( value );
		}
	}
	using math::is_positive;
	using math::is_negative;
}
