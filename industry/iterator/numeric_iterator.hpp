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
#include <cassert>
#include <industry/range.hpp>

namespace industry {
	template< typename NumT >
	class numeric_iterator {
		typedef numeric_iterator<NumT>       this_type;
	public:
		typedef std::forward_iterator_tag  iterator_category;
		typedef NumT                       value_type;
		typedef ptrdiff_t                  difference_type;
		typedef NumT*                      pointer;
		typedef NumT&                      reference;

		numeric_iterator() : value(), increment(), endValue() {}
		numeric_iterator( value_type const& value, value_type const& endValue, value_type const& increment)
			: value(value), increment(increment), endValue(endValue) { }

		this_type&   operator++() {
			if(value + increment < endValue) {
				value += increment;
			} else {
				if(value == endValue) {
					assert( false && "End iterator reached. Undefined behavior results from traversing past the end." );
				}

				value = endValue;
			}
			return *this;
		}

		this_type    operator++(int) { this_type copy( *this ); ++*this; return copy; }
		value_type   operator*() { return value; }
		pointer      operator->() { return &value; }

		friend bool operator==(const this_type& left, const this_type& right) {
			return left.value == right.value &&
				left.increment == right.increment &&
				left.endValue == right.endValue;
		}
		friend bool operator!=(const this_type& left, const this_type& right) {
			return !(left == right);
		}
	private:
		NumT value;
		NumT increment;
		NumT endValue;
	};

	template<typename NumT>
	range< numeric_iterator< NumT > > make_numeric_range(NumT begin, NumT end, NumT step = NumT(1)) {
		return range< numeric_iterator< NumT > >(
			numeric_iterator< NumT >(begin, end, step),
			numeric_iterator< NumT >(end, end, step)
		);
	}
}

#endif //IG_INDUSTRY_RANGE_NUMERIC_ITERATOR
