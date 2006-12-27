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

#include <cstdlib>

namespace industry {
	template < typename RangeT >
	struct range_traits {
		typedef typename RangeT::iterator        iterator;
		typedef typename RangeT::const_iterator  const_iterator;

		static iterator begin(RangeT& range) { return range.begin(); }
		static iterator end(RangeT& range) { return range.end(); }

		static const_iterator begin(const RangeT& range) { return range.begin(); }
		static const_iterator end(const RangeT& range) { return range.end(); }
	};

	template < typename RangeT >
	struct range_traits< const RangeT > {
		typedef typename RangeT::const_iterator  iterator;
		typedef typename RangeT::const_iterator  const_iterator;

		static const_iterator begin(const RangeT& range) { return range.begin(); }
		static const_iterator end(const RangeT& range) { return range.end(); }
	};

	template < typename RangeT, int S >
	struct range_traits< RangeT[S] > {
		typedef RangeT*                          iterator;
		typedef const RangeT*                    const_iterator;

		static iterator begin(RangeT range[S]) { return range; }
		static iterator end(RangeT range[S]) { return range + S; }

		static const_iterator begin(const RangeT range[S]) { return range; }
		static const_iterator end(const RangeT range[S]) { return range + S; }
	};

	template < typename RangeT, int S >
	struct range_traits< const RangeT[S] > {
		typedef const RangeT*                   iterator;
		typedef const RangeT*                   const_iterator;

		static const_iterator begin(const RangeT range[S]) { return range; }
		static const_iterator end(const RangeT range[S]) { return range + S; }
	};
}

#endif //ndef IG_INDUSTRY_TRAITS_RANGE
