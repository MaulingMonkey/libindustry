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
	namespace virtual_forward_iterator_detail {
		template < typename Value >
		struct impl_interface {
			virtual ~impl_interface() {}
			
			virtual Value& get_reference() const = 0;
			virtual Value* get_pointer()   const = 0;
			virtual std::auto_ptr< impl_interface< Value > > clone() const = 0;
			virtual void increment() = 0;
			virtual bool equals( const impl_interface< Value > & other ) const = 0;
		};
		
		template < typename Value , typename ForwardIterator >
		class impl : public impl_interface< Value > {
			ForwardIterator i;
		public:
			impl( const ForwardIterator & i ): i(i) {}
			
			virtual Value& get_reference() const { return *i; }
			virtual Value* get_pointer()   const { return &*i; }
			virtual std::auto_ptr< impl_interface< Value > > clone() const {
				return std::auto_ptr< impl_interface< Value > >( new impl(*this) );
			}
			virtual void increment() {
				++i;
			}
			virtual bool equals( const impl_interface< Value > & other ) const {
				assert( typeid(*this) == typeid(other) );
				return i == static_cast< const impl & >( other ).i;
			}
		};
	}
	
	template < typename Value >
	class virtual_forward_iterator {
		typedef virtual_forward_iterator< Value > self_t;
		
		boost::scoped_ptr< virtual_forward_iterator_detail::impl_interface< Value > > i;
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef std::ptrdiff_t            difference_type;
		typedef Value                     value_type;
		typedef Value&                    reference;
		typedef Value*                    pointer;
	
		virtual_forward_iterator(): i() {}
		virtual_forward_iterator( const virtual_forward_iterator & copy ): i(copy.i->clone()) { assert(i); }

		template < typename ForwardIterator >
		virtual_forward_iterator( const ForwardIterator & iter )
			:i( new virtual_forward_iterator_detail::impl< Value , ForwardIterator >(iter) )
		{}
		
		reference operator*() const { return i->get_reference(); }
		pointer   operator->() const { return i->get_pointer(); }
		
		self_t & operator++()    { assert(i); i->increment(); return *this; }
		self_t   operator++(int) { assert(i); self_t copy(*this); i->increment(); return copy; }
		
		friend bool operator==( const self_t & lhs , const self_t & rhs ) { assert( lhs.i && rhs.i ); return lhs.i->equals(*rhs.i); }
		friend bool operator!=( const self_t & lhs , const self_t & rhs ) { assert( lhs.i && rhs.i ); return !(lhs==rhs); }
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_FORWARD
