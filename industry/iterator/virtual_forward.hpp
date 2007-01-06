// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 31, 2006 - Created

#ifndef IG_INDUSTRY_ITERATOR_VIRTUAL_FORWARD
#define IG_INDUSTRY_ITERATOR_VIRTUAL_FORWARD

#include <boost/scoped_ptr.hpp>
#include <iterator>
#include <new>
#include <cassert>

#ifdef _DEBUG
#include <typeinfo>
#endif

namespace industry {
	namespace virtual_iterator_detail {
		template < typename Interface , typename ForwardIterator > class forward_impl;
	
		template < typename ValueT >
		class forward_impl_interface {
			typedef forward_impl_interface< ValueT > self;
		public:
			typedef ValueT value_type;
			
			virtual ~forward_impl_interface() {}
			
			virtual value_type& get_reference() const = 0;
			virtual value_type* get_pointer()   const = 0;
			virtual forward_impl_interface< value_type > * clone() const = 0;
			virtual void increment() = 0;
			virtual bool equals( const forward_impl_interface< value_type > & other ) const = 0;
			
			template < typename ForwardIterator > struct with_iterator {
				typedef forward_impl< self , ForwardIterator > type;
			};
		};
		
		
		
		template < typename Interface , typename ForwardIterator >
		class forward_impl : public Interface {
		protected:
			ForwardIterator i;
			typedef typename Interface::value_type value_type;
		public:
			forward_impl( const ForwardIterator & i ): i(i) {}
			
			virtual value_type& get_reference() const { return *i; }
			virtual value_type* get_pointer()   const { return &*i; }
			virtual Interface * clone() const {
				return new forward_impl(*this);
			}
			virtual void increment() { ++i; }
			virtual bool equals( const forward_impl_interface< value_type > & other ) const {
				assert( typeid(*this) == typeid(other) );
				return i == static_cast< const forward_impl & >( other ).i;
			}
		};
		
		
		
		template < typename Self , typename Value , typename Interface >
		class virtual_base {
			typedef Interface   iterator_interface_type;
		protected:
			boost::scoped_ptr< iterator_interface_type > i;
			virtual_base(): i() {}
			virtual_base( const virtual_base & copy ): i(copy.i->clone()) { assert(i); }
			virtual_base( iterator_interface_type * steal ): i(steal) { assert(i); }
		public:
			typedef std::ptrdiff_t              difference_type;
			typedef Value                       value_type;
			typedef value_type&                 reference;
			typedef value_type*                 pointer; 
		};
		
		
		
		template < typename Self , typename Value , typename Interface >
		class forward_base : public virtual_base< Self , Value , Interface > {
			Self & self()             { return static_cast<       Self& >( *this ); }
			const Self & self() const { return static_cast< const Self& >( *this ); }
			typedef Interface            iterator_interface_type;
			typedef virtual_base< Self , Value , Interface > super;
		protected:
			using super::i;
		public:
			typedef typename super::reference  reference;
			typedef typename super::pointer    pointer;
			
			forward_base(): super() {}
			forward_base( const forward_base & copy ): super(copy) {}
			
			template < typename ForwardIterator >
			explicit forward_base( const ForwardIterator & iter )
				:super( new typename iterator_interface_type::template with_iterator< ForwardIterator >::type( iter ) )
			{}
			
			reference operator* () const { return i->get_reference(); }
			pointer   operator->() const { return i->get_pointer(); }
		
			Self & operator++()    { assert(i); i->increment(); return self(); }
			Self   operator++(int) { assert(i); Self copy(self()); i->increment(); return copy; }
		
			friend bool operator==( const forward_base & lhs , const forward_base & rhs ) { assert( lhs.i && rhs.i ); return lhs.i->equals(*rhs.i); }
			friend bool operator!=( const forward_base & lhs , const forward_base & rhs ) { assert( lhs.i && rhs.i ); return !(lhs==rhs); }
		};
	}
	
	
	
	#define SELF      virtual_forward_iterator< Value >
	#define INTERFACE ::industry::virtual_iterator_detail::forward_impl_interface< Value >
	#define SUPER     ::industry::virtual_iterator_detail::forward_base< SELF , Value , INTERFACE >
	template < typename Value >
	class virtual_forward_iterator : public SUPER {
		typedef SELF  self;
		typedef SUPER super;
	#undef SELF
	#undef INTERFACE
	#undef SUPER
	public:
		typedef std::forward_iterator_tag              iterator_category;
		typedef Value                                  value_type;
		
		virtual_forward_iterator(): super() {}
		virtual_forward_iterator( const virtual_forward_iterator & copy ): super( static_cast< const super & >(copy) ) {}
		template < typename ForwardIterator > explicit virtual_forward_iterator( const ForwardIterator & iter ): super(iter) {}
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_FORWARD
