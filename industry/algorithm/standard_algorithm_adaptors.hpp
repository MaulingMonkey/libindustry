// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_STANDARD_ALGORITHM_ADAPTORS
#define IG_INDUSTRY_ALGORITHM_STANDARD_ALGORITHM_ADAPTORS

#include <algorithm>
#include <functional>
#include <cassert>
#include <industry/traits/range.hpp>

namespace industry {
	namespace algorithm {
		template<class RangeT, class OutputIterator>
		OutputIterator copy(RangeT const& range_, OutputIterator out) {
			return std::copy(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), out);
		}

		template<class RangeT, class BidirectionalIterator>
		BidirectionalIterator copy_backward(RangeT const& range_, BidirectionalIterator out) {
			return std::copy_backward(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), out);
		}

		template<class RangeT, class Type>
		typename std::iterator_traits<typename range_traits<RangeT>::iterator>::difference_type count(RangeT const& range_, const Type& value) {
			return std::count(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), value);
		}

		template<class RangeT, class Predicate>
		typename std::iterator_traits<typename range_traits<RangeT>::iterator>::difference_type count_if(RangeT const& range_, Predicate predicate) {
			return std::count_if(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT, class InputIterator, class BinaryPredicate>
		bool equal(RangeT const& range_, InputIterator begin) {
			return std::equal(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), begin, std::equal_to<typename std::iterator_traits<InputIterator>::value_type>());
		}

		template<class RangeT, class InputIterator, class BinaryPredicate>
		bool equal(RangeT const& range_, InputIterator begin, BinaryPredicate predicate) {
			return std::equal(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), begin, predicate);
		}

		template<class RangeT, class Type>
		void fill(RangeT& range_, const Type& value) {
			std::fill(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), value);
		}

		template<class RangeT, class Size, class Type>
		void fill_n(RangeT& range_, Size size, const Type& value) {
			assert(size <= range_traits<RangeT>::end(range_) - range_traits<RangeT>::begin(range_));
			std::fill_n(range_traits<RangeT>::begin(range_), size, value);
		}

		template<class RangeT1, class RangeT2>
		typename range_traits<RangeT1>::iterator find_end(RangeT1 const& range1, RangeT2 const& range2) {
			return find_end(range1, range2, std::equal_to<typename std::iterator_traits<typename range_traits<RangeT1>::iterator>::value_type>());
		}

		template<class RangeT1, class RangeT2, class BinaryPredicate>
		typename range_traits<RangeT1>::iterator find_end(RangeT1 const& range1, RangeT2 const& range2, BinaryPredicate predicate) {
			return std::find_end(range_traits<RangeT1>::begin(range1), range_traits<RangeT1>::end(range1), range_traits<RangeT2>::begin(range2), range_traits<RangeT2>::end(range2), predicate);
		}

		template<class RangeT1, class RangeT2>
		typename range_traits<RangeT1>::iterator find_first_of(RangeT1 const& range1, RangeT2 const& range2) {
			return find_first_of(range1, range2, std::equal_to<typename std::iterator_traits<typename range_traits<RangeT1>::iterator>::value_type>());
		}

		template<class RangeT1, class RangeT2, class BinaryPredicate>
		typename range_traits<RangeT1>::iterator find_first_of(RangeT1 const& range1, RangeT2 const& range2, BinaryPredicate predicate) {
			return std::find_first_of(range_traits<RangeT1>::begin(range1), range_traits<RangeT1>::end(range1), range_traits<RangeT2>::begin(range2), range_traits<RangeT2>::end(range2), predicate);
		}

		template<class RangeT, class Predicate>
		typename range_traits<RangeT>::iterator find_if(RangeT& range_, Predicate predicate) {
			return std::find_if(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT, class Function>
		Function for_each(RangeT& range_, Function function) {
			return std::for_each(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), function);
		}

		template<class RangeT, class Generator>
		void generate(RangeT& range_, Generator generator) {
			std::generate(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), generator);
		}

		template<class RangeT, class Size, class Generator>
		void generate_n(RangeT& range_, Size size, Generator generator) {
			assert(size <= range_traits<RangeT>::end(range_) - range_traits<RangeT>::begin(range_));
			std::generate_n(range_traits<RangeT>::begin(range_), size, generator);
		}

		template<class RangeT1, class RangeT2>
		typename range_traits<RangeT1>::iterator includes(RangeT1 const& range1, RangeT2 const& range2) {
			return includes(range1, range2, std::equal_to<typename std::iterator_traits<typename range_traits<RangeT1>::iterator>::value_type>());
		}

		template<class RangeT1, class RangeT2, class BinaryPredicate>
		typename range_traits<RangeT1>::iterator includes(RangeT1 const& range1, RangeT2 const& range2, BinaryPredicate predicate) {
			return std::includes(range_traits<RangeT1>::begin(range1), range_traits<RangeT1>::end(range1), range_traits<RangeT2>::begin(range2), range_traits<RangeT2>::end(range2), predicate);
		}

		template<class RangeT1, class RangeT2>
		bool lexicographical_compare(RangeT1 const& range1, RangeT2 const& range2) {
			return lexicographical_compare(range1, range2, std::equal_to<typename std::iterator_traits<typename range_traits<RangeT1>::iterator>::value_type>());
		}

		template<class RangeT1, class RangeT2, class BinaryPredicate>
		bool lexicographical_compare(RangeT1 const& range1, RangeT2 const& range2, BinaryPredicate predicate) {
			return std::lexicographical_compare(range_traits<RangeT1>::begin(range1), range_traits<RangeT1>::end(range1), range_traits<RangeT2>::begin(range2), range_traits<RangeT2>::end(range2), predicate);
		}

		template<class RangeT>
		void make_heap(RangeT const& range) {
			make_heap(range, std::less<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		void make_heap(RangeT const& range_, BinaryPredicate predicate) {
			return std::make_heap(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT>
		typename range_traits<RangeT>::iterator max_element(RangeT const& range) {
			return max_element(range, std::greater<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		typename range_traits<RangeT>::iterator max_element(RangeT const& range_, BinaryPredicate predicate) {
			return std::max_element(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT1, class RangeT2, class OutputIterator>
		bool merge(RangeT1 const& range1, RangeT2 const& range2, OutputIterator out) {
			return merge(range1, range2, out, std::equal_to<typename std::iterator_traits<typename range_traits<RangeT1>::iterator>::value_type>());
		}

		template<class RangeT1, class RangeT2, class OutputIterator, class BinaryPredicate>
		bool merge(RangeT1 const& range1, RangeT2 const& range2, OutputIterator out, BinaryPredicate predicate) {
			return std::merge(range_traits<RangeT1>::begin(range1), range_traits<RangeT1>::end(range1), range_traits<RangeT2>::begin(range2), range_traits<RangeT2>::end(range2), out, predicate);
		}

		template<class RangeT>
		typename range_traits<RangeT>::iterator min_element(RangeT const& range) {
			return min_element(range, std::less<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		typename range_traits<RangeT>::iterator min_element(RangeT const& range_, BinaryPredicate predicate) {
			return std::min_element(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT, class InputIterator>
		std::pair<typename range_traits<RangeT>::iterator, InputIterator> mismatch(RangeT const& range_, InputIterator first) {
			return mismatch(range_, std::greater<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class InputIterator, class BinaryPredicate>
		std::pair<typename range_traits<RangeT>::iterator, InputIterator> mismatch(RangeT const& range_, InputIterator first, BinaryPredicate predicate) {
			return std::mismatch(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), first, predicate);
		}

		template<class RangeT>
		bool next_permutation(RangeT& range) {
			return next_permutation(range, std::less<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		bool next_permutation(RangeT& range_, BinaryPredicate predicate) {
			return std::next_permutation(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT, class BinaryPredicate>
		void partition(RangeT& range_, BinaryPredicate predicate) {
			return std::partition(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT>
		void pop_heap(RangeT& range_) {
			pop_heap(range_, std::less<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		void pop_heap(RangeT& range_, BinaryPredicate predicate) {
			std::pop_heap(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}

		template<class RangeT>
		bool prev_permutation(RangeT& range) {
			return prev_permutation(range, std::less<typename std::iterator_traits<typename range_traits<RangeT>::iterator>::value_type>());
		}

		template<class RangeT, class BinaryPredicate>
		bool prev_permutation(RangeT& range_, BinaryPredicate predicate) {
			return std::prev_permutation(range_traits<RangeT>::begin(range_), range_traits<RangeT>::end(range_), predicate);
		}
	}

	using namespace algorithm;
}
#endif //IG_INDUSTRY_ALGORITHM_STANDARD_ALGORITHM_ADAPTORS
