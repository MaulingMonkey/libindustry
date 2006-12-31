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
		template < typename ValueT >
		struct forward_impl_interface {
			typedef ValueT value_type;
			
			virtual ~forward_impl_interface() {}
			
			virtual value_type& get_reference() const = 0;
			virtual value_type* get_pointer()   const = 0;
			virtual std::auto_ptr< forward_impl_interface< value_type > > clone() const = 0;
			virtual void increment() = 0;
			virtual bool equals( const forward_impl_interface< value_type > & other ) const = 0;
		};
		
		template < typename Interface , typename ForwardIterator >
		class forward_impl : public Interface {
			ForwardIterator i;
		protected:
			typedef typename Interface::value_type value_type;
		public:
			forward_impl( const ForwardIterator & i ): i(i) {}
			
			virtual value_type& get_reference() const { return *i; }
			virtual value_type* get_pointer()   const { return &*i; }
			virtual std::auto_ptr< forward_impl_interface< value_type > > clone() const {
				return std::auto_ptr< forward_impl_interface< value_type > >( new forward_impl(*this) );
			}
			virtual void increment() {
				++i;
			}
			virtual bool equals( const forward_impl_interface< value_type > & other ) const {
				assert( typeid(*this) == typeid(other) );
				return i == static_cast< const forward_impl & >( other ).i;
			}
		};
		template < typename Value >
		class forward_base {
			typedef forward_base< Value > self_t;
		protected:
			boost::scoped_ptr< forward_impl_interface< Value > > i;
		public:
			typedef std::ptrdiff_t            difference_type;
			typedef Value                     value_type;
			typedef Value&                    reference;
			typedef Value*                    pointer;

			forward_base(): i() {}
			forward_base( const forward_base & copy ): i(copy.i->clone()) { assert(i); }
			
			template < typename ForwardIterator >
			explicit forward_base( const ForwardIterator & iter )
				:i( new virtual_iterator_detail::forward_impl< virtual_iterator_detail::forward_impl_interface< Value > , ForwardIterator >(iter) )
			{}
			
			reference operator*() const { return i->get_reference(); }
			pointer   operator->() const { return i->get_pointer(); }
		
			self_t & operator++()    { assert(i); i->increment(); return *this; }
			self_t   operator++(int) { assert(i); self_t copy(*this); i->increment(); return copy; }
		
			friend bool operator==( const self_t & lhs , const self_t & rhs ) { assert( lhs.i && rhs.i ); return lhs.i->equals(*rhs.i); }
			friend bool operator!=( const self_t & lhs , const self_t & rhs ) { assert( lhs.i && rhs.i ); return !(lhs==rhs); }
		};
	}
	
	template < typename Value >
	class virtual_forward_iterator : public virtual_iterator_detail::forward_base< Value > {
		typedef virtual_forward_iterator< Value > self_t;
		typedef virtual_iterator_detail::forward_base< Value > super;
	public:
		typedef std::forward_iterator_tag iterator_category;
	
		template < typename ForwardIterator >
		explicit virtual_forward_iterator( const ForwardIterator & iter ): super(iter) {}
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_FORWARD
