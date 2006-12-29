// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_TRANSFORM
#define IG_INDUSTRY_ALGORITHM_TRANSFORM

#include <algorithm>
#include <iterator>
#include <boost/iterator.hpp>
#include <boost/type_traits.hpp>
#include <industry/traits/range.hpp>

namespace industry {
	namespace algorithm {
		template< typename FunctionS, typename HeldIterator >
		class transform_iterator_adaptor : public boost::iterator_facade<
			transform_iterator_adaptor<FunctionS, HeldIterator>,
			typename boost::remove_pointer<typename boost::function< FunctionS >::result_type>::type,
			typename std::iterator_traits<HeldIterator>::iterator_category
		> {
			typedef boost::function< FunctionS > function_type;
		public:
			transform_iterator_adaptor(function_type function, HeldIterator itor) : function(function), itor(itor){ }
			typedef typename std::iterator_traits<HeldIterator>::difference_type difference_type;
		private:
			friend class boost::iterator_core_access;

			template <typename FunctionOther, typename HeldItorOther>
			bool equal(transform_iterator_adaptor<FunctionOther, HeldItorOther> const& other) const
			{ 
				return this->itor == other.itor;
			}

			void increment() {
				++itor;
			}

			void decrement() {
				--itor;
			}

			void advance(difference_type n) {
				itor += n;
			}

			template <typename FunctionOther, typename HeldItorOther>
			difference_type distance_to(transform_iterator_adaptor<FunctionOther, HeldItorOther> const& other) const {
				return std::distance(itor, other.itor);
			}

			typename boost::remove_pointer<typename function_type::result_type>::type& dereference() const {
				return *function(&*itor);
			}

			function_type function;
			HeldIterator itor;
		};

		template< typename FunctionS >
		struct transform_proxy {
			transform_proxy(boost::function<FunctionS> function) : function(function) {}

			template< typename RangeT >
			friend range< transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> >
				operator|(RangeT & range, transform_proxy<FunctionS> const& proxy)
			{
				typedef transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.function, range_traits<RangeT>::begin(range)), iterator_type(proxy.function, range_traits<RangeT>::end(range)));
			}

			template< typename RangeT >
			friend range< transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> >
				operator|(RangeT const & range, transform_proxy<FunctionS> const& proxy)
			{
				typedef transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.function, range_traits<RangeT>::begin(range)), iterator_type(proxy.function, range_traits<RangeT>::end(range)));
			}
		private:
			boost::function<FunctionS> function;
		};

		template< typename R, typename A1 >
		transform_proxy<R (A1)> transform(R (*function)(A1)) {
			return transform_proxy<R (A1)>(function);
		}

		template< typename R, typename C >
		transform_proxy<R (C*)> transform(R (C::*function)()) {
			return transform_proxy<R (C*)>(function);
		}

		template< typename R, typename A1 >
		call_processor< R (A1) > transform( boost::function<R (A1)> function) {
			return transform_proxy< R (A1) >(function);
		}
	}

	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_TRANSFORM
