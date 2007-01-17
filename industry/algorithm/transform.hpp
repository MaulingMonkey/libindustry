// Copyright (c) 2006-2007 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Adopted changes from deffer (GDNet) 

#ifndef IG_INDUSTRY_ALGORITHM_TRANSFORM
#define IG_INDUSTRY_ALGORITHM_TRANSFORM

#include <algorithm>
#include <iterator>
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits.hpp>
#include <industry/traits/range.hpp>

namespace industry {
	namespace algorithm {
		template< typename FunctionS, typename HeldIterator >
		class transform_iterator_adaptor;

		template < class FunctionS, class HeldIterator >
		struct transform_iterator_adaptor_base
		{
			typedef boost::iterator_adaptor<
				transform_iterator_adaptor< FunctionS, HeldIterator >
				, HeldIterator
				, typename boost::remove_reference< typename boost::function< FunctionS >::result_type >::type
				, typename std::iterator_traits< HeldIterator >::iterator_category
				, typename boost::function< FunctionS >::result_type
			> type;
		};

		template< typename FunctionS, typename HeldIterator >
		class transform_iterator_adaptor : public transform_iterator_adaptor_base<FunctionS, HeldIterator>::type
		{
			typedef boost::function< FunctionS > function_type;
			typedef typename boost::function< FunctionS >::result_type dereferenced_type;
			typedef typename transform_iterator_adaptor_base< FunctionS, HeldIterator >::type base_type;
		public:
			transform_iterator_adaptor(function_type function, HeldIterator itor) : base_type(itor), function(function) { }
		private:
			friend class boost::iterator_core_access;

			dereferenced_type dereference() const {
				return function(*this->base_reference());
			}

			function_type function;
		};

		template< typename FunctionS >
		struct transform_proxy {
			transform_proxy(boost::function<FunctionS> function) : function(function) {}

			template< typename RangeT >
			friend range< transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> >
				operator|(RangeT & range_, transform_proxy<FunctionS> const& proxy)
			{
				typedef transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.function, range_traits<RangeT>::begin(range_)), iterator_type(proxy.function, range_traits<RangeT>::end(range_)));
			}

			template< typename RangeT >
			friend range< transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::const_iterator> >
				operator|(RangeT const & range_, transform_proxy<FunctionS> const& proxy)
			{
				typedef transform_iterator_adaptor<FunctionS, typename range_traits<RangeT>::const_iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.function, range_traits<RangeT>::begin(range_)), iterator_type(proxy.function, range_traits<RangeT>::end(range_)));
			}
		private:
			boost::function<FunctionS> function;
		};

		template< typename T, typename From >
		struct copy_const {
			typedef T type;
		};
		template< typename T, typename From_ >
		struct copy_const<T, const From_> {
			typedef const T type;
		};

		template< typename Res, typename Obj, typename HeldIterator >
		class transform_memberptr_iterator_adaptor;

		template < typename Res, typename Obj, class HeldIterator >
		struct transform_memberptr_iterator_adaptor_base
		{
			typedef boost::iterator_adaptor<
				transform_memberptr_iterator_adaptor< Res, Obj, HeldIterator >
				, HeldIterator
				, Res // value_type
				, typename std::iterator_traits< HeldIterator >::iterator_category
				, typename copy_const<
				Res,
				typename boost::remove_pointer<
				typename boost::remove_reference<
				typename std::iterator_traits<HeldIterator>::reference >::type >::type
				>::type & // reference_type - add const if the original type is const
			> type;
		};

		template< typename Res, typename Obj, typename HeldIterator >
		class transform_memberptr_iterator_adaptor : public transform_memberptr_iterator_adaptor_base<Res, Obj, HeldIterator>::type
		{
			typedef Res Obj::*memberptr_type;
			typedef transform_memberptr_iterator_adaptor<Res, Obj, HeldIterator> this_type;
			typedef typename transform_memberptr_iterator_adaptor_base<Res, Obj, HeldIterator>::type base_type;
			typedef typename base_type::reference  dereferenced_type;
		public:
			transform_memberptr_iterator_adaptor(memberptr_type memberptr, HeldIterator itor) : base_type(itor), memberptr(memberptr) { }
		private:
			friend class boost::iterator_core_access;

			template< typename Obj_ >
			struct apply_dereference
			{
				static dereferenced_type apply(const this_type & t) {
					return (*t.base_reference()).*(t.memberptr);
				};
			};
			template< typename Obj_ >
			struct apply_dereference< Obj_* >
			{
				static dereferenced_type apply(const this_type & t) {
					return (*t.base_reference())->*(t.memberptr);
				};
			};
			typedef apply_dereference< typename std::iterator_traits<HeldIterator>::value_type > dereference_applier;

			dereferenced_type dereference() const {
				return dereference_applier::apply(*this);
			}

			const memberptr_type memberptr;
		};

		template< typename Res, typename Obj >
		struct transform_memberptr_proxy {
			typedef Res Obj::*memberptr_type;

			transform_memberptr_proxy(memberptr_type memberptr) : memberptr(memberptr) {}

			template< typename RangeT >
			friend range< transform_memberptr_iterator_adaptor<Res, Obj, typename range_traits<RangeT>::iterator> >
				operator|(RangeT & range_, transform_memberptr_proxy<Res,Obj> const& proxy)
			{
				typedef transform_memberptr_iterator_adaptor<Res, Obj, typename range_traits<RangeT>::iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.memberptr, range_traits<RangeT>::begin(range_)), iterator_type(proxy.memberptr, range_traits<RangeT>::end(range_)));
			}

			template< typename RangeT >
			friend range< transform_memberptr_iterator_adaptor<Res, Obj, typename range_traits<RangeT>::const_iterator> >
				operator|(RangeT const & range_, transform_memberptr_proxy<Res,Obj> const& proxy)
			{
				typedef transform_memberptr_iterator_adaptor<Res, Obj, typename range_traits<RangeT>::const_iterator> iterator_type;
				return industry::range<iterator_type>(iterator_type(proxy.memberptr, range_traits<RangeT>::begin(range_)), iterator_type(proxy.memberptr, range_traits<RangeT>::end(range_)));
			}
		private:
			const memberptr_type memberptr;
		};


		template< typename Res, typename Obj >
		transform_memberptr_proxy<Res, Obj> transform(Res Obj::*memberptr) {
			return transform_memberptr_proxy<Res, Obj>(memberptr);
		}

		template< typename R, typename A1 >
		transform_proxy<R (A1)> transform(R (*function)(A1)) {
			return transform_proxy<R (A1)>(function);
		}

		template< typename R, typename A1 >
		transform_proxy< R (A1) > transform( boost::function<R (A1)> function) {
			return transform_proxy< R (A1) >(function);
		}

		template< typename R, typename C >
		transform_proxy<R (C&)> transform(R (C::*function)()) {
			return transform_proxy<R (C&)>(function);
		}

		template< typename R, typename C >
		transform_proxy<R (const C&)> transform(R (C::*function)() const) {
			return transform_proxy<R (const C&)>(function);
		}
	}

	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_TRANSFORM
