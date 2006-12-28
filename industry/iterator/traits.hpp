// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 28, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ITERATOR_TRAITS
#define IG_INDUSTRY_ITERATOR_TRAITS

#include <iterator>

namespace industry {
	template < typename Iterator >
	struct iterator_traits_base : std::iterator_traits< Iterator > {
	};
	
	template < typename Iterator >
	struct iterator_traits : iterator_traits_base< Iterator > {
		typedef typeof(*Iterator())              result_type;
		typedef typeof(Iterator().operator->())  ptr_result_type;
	};
	
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
