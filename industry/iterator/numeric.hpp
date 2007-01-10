// Copyright (c) 2006-2007 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 26, 2006 - Created

#ifndef IG_INDUSTRY_RANGE_NUMERIC_ITERATOR
#define IG_INDUSTRY_RANGE_NUMERIC_ITERATOR

#include <boost/iterator/iterator_facade.hpp>
#include <iterator>
#include <cassert>

namespace std {
	template<class T>
	struct equal_to;
}

namespace industry {
	template<typename ValueT, typename StepT = ValueT, typename EqualityP = std::equal_to<ValueT> >
	class numeric_iterator : public boost::iterator_facade<
		numeric_iterator<ValueT, StepT, EqualityP>,
		ValueT,
		std::random_access_iterator_tag,
		ValueT >
	{
		typedef boost::iterator_facade<numeric_iterator<ValueT, StepT, EqualityP>, ValueT, std::random_access_iterator_tag> base_type;
	public:
		numeric_iterator() : infinite(true) {}
		numeric_iterator(ValueT value, StepT step, EqualityP equality_comparer = EqualityP()) : equality_comparer(equality_comparer), value(value), step(step), infinite(false) {}
	private:
		friend class boost::iterator_core_access;

		ValueT dereference() const {
			return value;
		}

		template<typename YValueT, typename YStepT, typename YEqualityP>
		bool equal(numeric_iterator<YValueT, YStepT, YEqualityP> const& other) const {
			return infinite || other.infinite ? false : equality_comparer(value, ValueT(other.value));
		}

		void increment() {
			assert(value + step > value);
			value += step;
		}

		void decrement() {
			assert(value - step < value);
			value -= step;
		}

		void advance(typename base_type::difference_type n) {
			assert(value + n * step > value);
			value += n * step;
		}

		template<typename YValueT, typename YStepT, typename YEqualityP>
		typename base_type::difference_type distance_to(numeric_iterator<YValueT, YStepT, YEqualityP> const& other) const {
			if(infinite)
				return -std::numeric_limits<typename base_type::difference_type>::max();
			else if(other.infinite)
				return std::numeric_limits<typename base_type::difference_type>::min();
			else
				return typename base_type::difference_type((other.value - value) / step);
		}

		EqualityP equality_comparer;
		ValueT value;
		StepT step;
		const bool infinite;
	};
}

#endif //IG_INDUSTRY_RANGE_NUMERIC_ITERATOR
