// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  5, 2007 - Created

#ifndef IG_INDUSTRY_ITERATOR_VIRTUAL_BIDIRECTIONAL
#define IG_INDUSTRY_ITERATOR_VIRTUAL_BIDIRECTIONAL

#include <industry/iterator/virtual_forward.hpp>
#include <boost/scoped_ptr.hpp>
#include <iterator>
#include <new>
#include <cassert>

namespace industry {
	namespace iterators {
		namespace detail {
			template < typename Self , typename Value , typename Interface >
			class bidirectional_base : public forward_base< Self , Value , Interface > {
				Self & self()             { return static_cast<       Self& >( *this ); }
				const Self & self() const { return static_cast< const Self& >( *this ); }
				typedef Interface            iterator_interface_type;
				typedef forward_base< Self , Value , Interface > super;
			protected:
				using super::i;
			public:
				typedef typename super::reference  reference;
				typedef typename super::pointer    pointer;
				
				bidirectional_base(): super() {}
				bidirectional_base( const bidirectional_base & copy ): super( static_cast< const super& >( copy ) ) {}
				
				template < typename BidirectionalIterator >
				explicit bidirectional_base( const BidirectionalIterator & iter ): super(iter) {}
				
				Self & operator--()    { assert(i); i->move_prev(); return self(); }
				Self   operator--(int) { assert(i); Self copy(self()); i->move_prev(); return copy; }
			};
		}
		#define SELF      virtual_bidirectional_iterator< Value >
		#define INTERFACE ::industry::iterators::detail::virtual_impl_interface< Value , std::bidirectional_iterator_tag >
		#define SUPER     ::industry::iterators::detail::bidirectional_base< SELF , Value , INTERFACE >
		template < typename Value >
		class virtual_bidirectional_iterator : public SUPER {
			typedef SELF  self;
			typedef SUPER super;
		#undef SELF
		#undef INTERFACE
		#undef SUPER
		public:
			typedef std::bidirectional_iterator_tag        iterator_category;
			typedef Value                                  value_type;
			
			virtual_bidirectional_iterator(): super() {}
			virtual_bidirectional_iterator( const virtual_bidirectional_iterator & copy ): super( static_cast< const super & >(copy) ) {}
			template < typename BidirectionalIterator > explicit virtual_bidirectional_iterator( const BidirectionalIterator & iter ): super(iter) {}
		};
	}
	using ::industry::iterators::virtual_bidirectional_iterator;
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_BIDIRECTIONAL
