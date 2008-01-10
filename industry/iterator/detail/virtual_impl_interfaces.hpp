// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  6, 2007 - Created

#ifndef IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL_INTERFACES
#define IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL_INTERFACES

#include <iterator>
#include <cstddef>

namespace industry {
	namespace iterators {
		namespace detail {
			template < typename Self , typename Value >
			struct virtual_basic_impl_interface {
				virtual ~virtual_basic_impl_interface() {}
				
				virtual Self*  clone() const = 0;
				virtual Value& reference() const = 0;
				virtual Value* pointer  () const = 0;
				virtual bool   equals( const virtual_basic_impl_interface & ) = 0;
			};
			struct virtual_forward_impl_interface {
				virtual ~virtual_forward_impl_interface() {}
				virtual void move_next() = 0;
			};
			struct virtual_bidirectional_impl_interface {
				virtual ~virtual_bidirectional_impl_interface() {}
				virtual void move_prev() = 0;
			};
			struct virtual_random_access_impl_interface {
				virtual ~virtual_random_access_impl_interface() {}
				virtual void move_by( ptrdiff_t ) = 0;
				virtual ptrdiff_t distance_to( const virtual_random_access_impl_interface & other ) const = 0;
			};
			
			template < typename Value , typename Category , typename Iterator > struct virtual_impl;
			template < typename Value , typename Category > struct virtual_impl_interface;
			
			template < typename Value > struct virtual_impl_interface< Value , std::forward_iterator_tag >
				: public virtual_basic_impl_interface< virtual_impl_interface< Value , std::forward_iterator_tag > , Value >
				, public virtual_forward_impl_interface
			{
				template < typename Iterator > struct with_iterator { typedef virtual_impl< Value , std::forward_iterator_tag , Iterator > type; };
			};
			
			template < typename Value > struct virtual_impl_interface< Value , std::bidirectional_iterator_tag >
				: public virtual_basic_impl_interface< virtual_impl_interface< Value , std::bidirectional_iterator_tag > , Value >
				, public virtual_forward_impl_interface
				, public virtual_bidirectional_impl_interface
			{
				template < typename Iterator > struct with_iterator { typedef virtual_impl< Value , std::bidirectional_iterator_tag , Iterator > type; };
			};
			
			template < typename Value > struct virtual_impl_interface< Value , std::random_access_iterator_tag >
				: public virtual_basic_impl_interface< virtual_impl_interface< Value , std::random_access_iterator_tag > , Value >
				, public virtual_forward_impl_interface
				, public virtual_bidirectional_impl_interface
				, public virtual_random_access_impl_interface
			{
				template < typename Iterator > struct with_iterator { typedef virtual_impl< Value , std::random_access_iterator_tag , Iterator > type; };
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL_INTERFACES
