// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 11, 2007 - Created

#ifndef IG_INDUSTRY_ATTRIBUTES_RANGE
#define IG_INDUSTRY_ATTRIBUTES_RANGE

#include <industry/traits/range.hpp>

namespace industry {
	namespace attributes {
		namespace range {
			template < typename Range >
			typename industry::range_traits< Range >::iterator begin( Range& range ) {
				return industry::range_traits< Range >::begin(range);
			}
			template < typename Range >
			typename industry::range_traits< Range >::iterator end( Range& range ) {
				return industry::range_traits< Range >::end(range);
			}
			template < typename Range >
			typename industry::range_traits< Range >::const_iterator begin( const Range& range ) {
				return industry::range_traits< Range >::begin(range);
			}
			template < typename Range >
			typename industry::range_traits< Range >::const_iterator end( const Range& range ) {
				return industry::range_traits< Range >::end(range);
			}
			template < typename Range >
			typename industry::range_traits< Range >::size_type size( const Range& range ) {
				return industry::range_traits< Range >::size_type(end(range)-begin(range));
			}
			template < typename T , std::size_t n >
			std::size_t size( const T (&) [n] ) { return n; }
		}
		namespace all {
			using namespace attributes::range;
		}
	}
}

#endif //ndef IG_INDUSTRY_ATTRIBUTES_RANGE
