// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Nov 11, 2006 - Created

#ifndef IG_INDUSTRY_ALGORITHM_PUSH
#define IG_INDUSTRY_ALGORITHM_PUSH

#if 1

#else 0 //Old header -- for reference only:
#include <algorithm>
#include <iterator>
#include <industry/traits/range.hpp>

namespace industry {
	namespace algorithm {
		template < typename ContainerT >
		class push_back_processor {
			ContainerT & container;
		public:
			push_back_processor( ContainerT & container ) : container( container ) {}

			template < typename RangeT >
			friend void operator|( RangeT & range_ , const push_back_processor< ContainerT > & p ) {
				std::copy( range_traits<RangeT>::begin(range_) , range_traits<RangeT>::end(range_) , std::back_inserter( p.container ) );
			}
			template < typename RangeT >
			friend void operator|( const RangeT & range_ , const push_back_processor< ContainerT > & p ) {
				std::copy( range_traits<RangeT>::begin(range_) , range_traits<RangeT>::end(range_) , std::back_inserter( p.container ) );
			}
		private:
			push_back_processor& operator=(push_back_processor const&);
		};

		template < typename ContainerT >
		push_back_processor< ContainerT > push_back( ContainerT & container ) {
			return push_back_processor< ContainerT >( container );
		}
	}
	using namespace algorithm;
}
#endif

#endif //ndef IG_INDUSTRY_ALGORITHM_PUSH
