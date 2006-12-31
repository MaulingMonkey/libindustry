// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
// Dec 29, 2006 - fixes, refactoring
// Dec 25, 2006 - industry.iterator.n.hpp => industry/iterator/n.hpp
// Nov 12, 2006 - Created

#ifndef IG_INDUSTRY_ITERATOR_N
#define IG_INDUSTRY_ITERATOR_N

#include <industry/iterator/cast_base.hpp>
#include <boost/call_traits.hpp>
#include <boost/optional.hpp>
#include <iterator>
#include <cassert>

namespace industry {
	template < typename IteratorT >
	class n_iterator {
		typedef n_iterator< IteratorT > this_t;
		
		static bool is_initialized( const this_t & iter ) { return iter.impl || iter.n; }
		static bool is_dereferenceable( const this_t & iter ) { return iter.impl; }
	protected:
		boost::optional< IteratorT > impl;
		size_t n;
	public:
		typedef typename std::forward_iterator_tag                               iterator_category;
		typedef typename std::iterator_traits< IteratorT >::value_type           value_type;
		typedef typename std::iterator_traits< IteratorT >::difference_type      difference_type;
		typedef typename std::iterator_traits< IteratorT >::pointer              pointer;
		typedef typename std::iterator_traits< IteratorT >::reference            reference;
		
		n_iterator() : impl() , n(0)                                                { /* --- uninitialized iterator --- */  }
		explicit n_iterator( const IteratorT & iterator ) : impl(iterator) , n(0)   { /* begin iterator */                  }
		n_iterator( const IteratorT & iterator , size_t n ) : impl(iterator) , n(n) { /* count-and-explicit end iterator */ }
		explicit n_iterator( size_t n ) : impl() , n(n)                             { /* count-only end iterator */         }
		n_iterator( const this_t & iterator ) : impl(iterator.impl) , n(iterator.n) { /* iterator copy */                   }

		this_t &    operator++()    { /* prefix  version */ assert( is_initialized(*this) ); ++impl.get(); ++n; return *this; }
		this_t      operator++(int) { /* postfix version */ assert( is_initialized(*this) ); this_t copy( *this ); ++impl.get(); ++n; return copy; }
		reference   operator* () const { assert( is_initialized(*this) && is_dereferenceable(*this) ); return impl.get().operator*(); }
		pointer     operator->() const { assert( is_initialized(*this) && is_dereferenceable(*this) ); return impl.get().operator->(); }
		
		friend bool operator==( const this_t & lhs , const this_t & rhs ) {
			assert( is_initialized(lhs) );
			assert( is_initialized(rhs) );
			
			return lhs.n == rhs.n || (lhs.impl && rhs.impl && lhs.impl.get() == rhs.impl.get());
		}
		friend bool operator!=( const this_t & lhs , const this_t & rhs ) {
			return !(lhs==rhs);
		}
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_N
