// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 28, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$
//
// FIXME -- Provide #define (and compiler defaults) to disable automatic catch-all case implementation
//          will require all relevant iterators have industry::iterator_traits specialized for their
//          iterator type
// TODO  -- Provide the aforementioned specializations for standard library iterators.

#ifndef IG_INDUSTRY_ITERATOR_TRAITS
#define IG_INDUSTRY_ITERATOR_TRAITS

#include <industry/config.hpp>
#include <industry/member_function_ptr_traits.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <iterator>

#if !defined( INDUSTRY_TYPEOF )
#include <set>
#include <map>
#include <list>
#include <vector>
#endif

namespace industry {
	template < typename Iterator >
	struct iterator_traits_base : std::iterator_traits< Iterator > {
	};
	
#if defined( INDUSTRY_TYPEOF )
	template < typename Iterator >
	struct iterator_traits : iterator_traits_base< Iterator > {
		typedef typename industry::member_function_ptr_traits< INDUSTRY_TYPEOF(&Iterator::operator* ) >::result_type   result_type;
		typedef typename industry::member_function_ptr_traits< INDUSTRY_TYPEOF(&Iterator::operator->) >::result_type   ptr_result_type;
	};
#else
	template < typename Iterator > struct iterator_traits;
	
	//TODO: Specialize iterator_traits for standard library iterators
#endif
		
	template < typename ValueT >
	struct iterator_traits< ValueT * > : iterator_traits_base< ValueT * > {
		typedef ValueT & result_type;
		typedef ValueT * ptr_result_type;
	};
	
	template < typename ValueT >
	struct iterator_traits< const ValueT * > : iterator_traits_base< const ValueT * > {
		typedef const ValueT & result_type;
		typedef const ValueT * ptr_result_type;
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_TRAITS
