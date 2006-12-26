// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.traits.range.hpp => industry/traits/range.hpp>
// Nov 12, 2006 - Created

#ifndef IG_INDUSTRY_TRAITS_RANGE
#define IG_INDUSTRY_TRAITS_RANGE

namespace industry {
	template < typename RangeT >
	struct range_traits {
		typedef typename RangeT::iterator        iterator;
		typedef typename RangeT::const_iterator  const_iterator;
	};

	template < typename RangeT >
	struct range_traits< const RangeT > {
		typedef typename RangeT::const_iterator  iterator;
		typedef typename RangeT::const_iterator  const_iterator;
	};
}

#endif //ndef IG_INDUSTRY_TRAITS_RANGE
