// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
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
	namespace virtual_iterator_detail {
		template < typename Interface , typename BidirectionalIterator > class bidirectional_impl;
	
		template < typename ValueT >
		class bidirectional_impl_interface : public forward_impl_interface< ValueT > {
			typedef bidirectional_impl_interface< ValueT > self;
		public:
			typedef ValueT value_type;
			
			virtual bidirectional_impl_interface< value_type > * clone() const = 0;
			virtual void decrement() = 0;
			
			template < typename BidirectionalIterator > struct with_iterator {
				typedef bidirectional_impl< self , BidirectionalIterator > type;
			};
		};
		
		
		
		template < typename Interface , typename BidirectionalIterator >
		class bidirectional_impl : public forward_impl< Interface , BidirectionalIterator > {
			typedef forward_impl< Interface , BidirectionalIterator > super;
		protected:
			using super::i;
			typedef typename Interface::value_type value_type;
		public:
			bidirectional_impl( const BidirectionalIterator & i ): super(i) {}
			
			virtual bidirectional_impl_interface< value_type > * clone() const {
				return new bidirectional_impl(*this);
			}
			virtual void decrement() { --i; }
		};
		
		
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
			bidirectional_base( const super & copy ): super(copy) {}
			bidirectional_base( const bidirectional_base & copy ): super(copy) {}
			
			template < typename BidirectionalIterator >
			explicit bidirectional_base( const BidirectionalIterator & iter )
				:super( new typename iterator_interface_type::template with_iterator< BidirectionalIterator >::type( iter ) )
			{}
			
			Self & operator--()    { assert(i); i->decrement(); return self(); }
			Self   operator--(int) { assert(i); Self copy(self()); i->decrement(); return copy; }
		};
	}
	
	
	
	#define SELF      virtual_bidirectional_iterator< Value >
	#define INTERFACE ::industry::virtual_iterator_detail::bidirectional_impl_interface< Value >
	#define SUPER     ::industry::virtual_iterator_detail::bidirectional_base< SELF , Value , INTERFACE >
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
		template < typename ForwardIterator > explicit virtual_bidirectional_iterator( const ForwardIterator & iter ): super(iter) {}
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_BIDIRECTIONAL
