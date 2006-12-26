// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dev 25, 2006 - industry.iterator.cast.dynamic.hpp => industry/iterator/dynamic_cast.hpp
// Nov 11, 2006 - Reimplemented in terms of cast_iterator_base
// Nov 10, 2006 - Created

#ifndef IG_INDUSTRY_ITERATOR_DYNAMIC_CAST
#define IG_INDUSTRY_ITERATOR_DYNAMIC_CAST

#include <industry/iterator/cast_base.hpp>
#include <boost/call_traits.hpp>

namespace industry {
	namespace casts {
		template < typename T , typename IteratorT >
		class dynamic_cast_iterator : public cast_iterator_base< dynamic_cast_iterator< T , IteratorT > , T , IteratorT > {
			typedef cast_iterator_base< dynamic_cast_iterator< T , IteratorT > , T , IteratorT > base_type;
			using base_type::impl;
		public:
			typedef typename base_type::difference_type difference_type;
			
			dynamic_cast_iterator() : base_type() {}
			explicit dynamic_cast_iterator( const IteratorT & iterator )   : base_type( iterator ) {}
			dynamic_cast_iterator( const dynamic_cast_iterator & iterator ) : base_type( iterator ) {}

			T   operator* () const { return  dynamic_cast< T >( *impl ); }
			T * operator->() const { return &dynamic_cast< T >( *impl ); }
			T   operator[]( difference_type difference ) const { return static_cast< T >( impl[difference] ); }
		};

		template < typename T , typename IteratorT >
		dynamic_cast_iterator<T,IteratorT> dynamic_iterator_cast( IteratorT iterator ) {
			return dynamic_cast_iterator<T,IteratorT>( iterator );
		}
	}
	using namespace casts;
}

#endif //ndef IG_INDUSTRY_ITERATOR_DYNAMIC_CAST
