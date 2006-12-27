// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_RANGE_NUMERIC_ITERATOR
#define IG_INDUSTRY_RANGE_NUMERIC_ITERATOR

#include <iterator>
#include <cstdlib>

namespace industry {
	namespace iterator {
		template< typename NumT >
		class finite_numeric_iterator {
		public:
			typedef std::bidirectional_iterator_tag            iterator_category;
			typedef NumT                                       value_type;
			typedef ptrdiff_t                                  difference_type;
			typedef NumT*                                      pointer;
			typedef NumT&                                      reference;
		};
	}
}

#endif //IG_INDUSTRY_RANGE_NUMERIC_ITERATOR