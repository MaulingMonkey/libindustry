// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - industry.iterator.case.base.hpp => industry/iterator/cast_base.hpp
// Nov 11, 2006 - Created

#ifndef IG_INDUSTRY_ITERATOR_CAST_BASE
#define IG_INDUSTRY_ITERATOR_CAST_BASE

#include <boost/call_traits.hpp>
#include <iterator>

namespace industry {
	namespace casts {
		template < typename SelfT , typename T , typename IteratorT >
		class cast_iterator_base {
		protected:
			IteratorT impl;
			const SelfT & self() const { return * static_cast< const SelfT * >( this ); }
			SelfT       & self()       { return * static_cast<       SelfT * >( this ); }
		public:
			typedef typename std::iterator_traits< IteratorT >::iterator_category    iterator_category;
			typedef typename boost::call_traits< T >::value_type                     value_type;
			typedef typename std::iterator_traits< IteratorT >::difference_type      difference_type;
			typedef typename boost::call_traits< T >::value_type *                   pointer;
			typedef typename boost::call_traits< T >::reference                      reference;
			
			cast_iterator_base() : impl() {}
			explicit cast_iterator_base( const IteratorT & iterator ) : impl( iterator ) {}
			cast_iterator_base( const cast_iterator_base & iterator ) : impl( iterator.impl ) {}

			SelfT & operator++()  { /* prefix  version */ ++impl; return self(); }
			SelfT & operator--()  { /* prefix  version */ --impl; return self(); }
			SelfT   operator++(int) { /* postfix version */ SelfT copy( *this ); ++impl; return copy; }
			SelfT   operator--(int) { /* postfix version */ SelfT copy( *this ); --impl; return copy; }
			SelfT & operator+=( difference_type difference ) { impl += difference; return self(); }
			SelfT & operator-=( difference_type difference ) { impl -= difference; return self(); }
			SelfT   operator+ ( difference_type difference ) { return SelfT( impl + difference ); }
			SelfT   operator- ( difference_type difference ) { return SelfT( impl - difference ); }
			difference_type operator-( const cast_iterator_base & rhs ) { return impl - rhs.impl; }

			//T       operator* () const { return  dynamic_cast< T >(  impl.operator* () ); }
			//pointer operator->() const { return &dynamic_cast< T >( *impl.operator->() ); }
			//T       operator[]( difference_type difference ) const { return static_cast< T >( impl[difference] ); }

			friend bool operator==( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl == rhs.impl;
			}
			friend bool operator!=( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl != rhs.impl;
			}
			friend bool operator<=( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl <= rhs.impl;
			}
			friend bool operator>=( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl >= rhs.impl;
			}
			friend bool operator< ( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl < rhs.impl;
			}
			friend bool operator> ( const cast_iterator_base & lhs , const cast_iterator_base & rhs ) {
				return lhs.impl > rhs.impl;
			}
		};
	}
	using namespace casts;
}

#endif //ndef IG_INDUSTRY_ITERATOR_CAST_BASE
