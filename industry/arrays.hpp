// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.utility.hpp => industry/arrays.hpp
// Nov 10, 2006 - Moved into industry::arrays namespace
// Sep 18, 2006 - Created

#ifndef IG_INDUSTRY_ARRAYS
#define IG_INDUSTRY_ARRAYS

namespace industry {
	namespace arrays {
		template < typename T , unsigned N > const T * begin( const T (&array)[N] ) { return array;     }
		template < typename T , unsigned N > const T * end  ( const T (&array)[N] ) { return array + N; }
		template < typename T , unsigned N > T *       begin( T       (&array)[N] ) { return array;     }
		template < typename T , unsigned N > T *       end  ( T       (&array)[N] ) { return array + N; }
		template < typename T , unsigned N > unsigned  size ( const T (&     )[N] ) { return N;         }
	}
	using namespace arrays;
}

#endif //ndef IG_INDUSTRY_ARRAYS
