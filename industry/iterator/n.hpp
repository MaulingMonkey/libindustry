// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
// Dec 25, 2006 - industry.iterator.n.hpp => industry/iterator/n.hpp
// Nov 12, 2006 - Created

#ifndef IG_INDUSTRY_ITERATOR_N
#define IG_INDUSTRY_ITERATOR_N

#include <industry/iterator/cast_base.hpp>
#include <boost/call_traits.hpp>
#include <boost/optional.hpp>
#include <iterator>

namespace industry {
	template < typename IteratorT >
	class n_iterator {
		typedef n_iterator< IteratorT > this_t;
	protected:
		boost::optional< IteratorT > impl;
		unsigned n;
	public:
		typedef typename std::forward_iterator_tag                               iterator_category;
		typedef typename std::iterator_traits< IteratorT >::value_type           value_type;
		typedef typename std::iterator_traits< IteratorT >::difference_type      difference_type;
		typedef typename std::iterator_traits< IteratorT >::pointer              pointer;
		typedef typename std::iterator_traits< IteratorT >::reference            reference;
		
		n_iterator() : impl() , n(0) { /* count-only end iterator */ }
		n_iterator( const IteratorT & iterator ) : impl(iterator) , n(0) { /* count-and-explicit end iterator */ }
		n_iterator( const IteratorT & iterator , unsigned n ) : impl(iterator) , n(n) { /* iterator */ }
		n_iterator( const this_t & iterator ) : impl(iterator.impl) , n(iterator.n) { /* iterator copy */ }

		this_t &    operator++()    { /* prefix  version */ ++impl.get(); --n; return *this; }
		this_t      operator++(int) { /* postfix version */ this_t copy( *this ); ++impl.get(); --n; return copy; }
		value_type  operator* () const { return  *impl.get(); }
		pointer     operator->() const { return &*impl.get(); }
		
		friend bool operator==( const this_t & lhs , const this_t & rhs ) {
			if ( lhs.impl && rhs.impl && lhs.n && rhs.n ) {
				return lhs.impl.get() == rhs.impl.get();
			} else {
				return (!lhs.impl || !lhs.n) && (!rhs.impl || !rhs.n);
			}
		}
		friend bool operator!=( const this_t & lhs , const this_t & rhs ) {
			return !(lhs==rhs);
		}
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_N
