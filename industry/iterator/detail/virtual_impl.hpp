// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  6, 2007 - Created

#ifndef IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL
#define IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL

#include <industry/iterator/detail/virtual_impl_interfaces.hpp>
#include <cstddef>

#ifdef _DEBUG
#include <typeinfo>
#endif

namespace industry {
	namespace iterators {
		namespace detail {
			template < typename Self , typename Value , typename Category , typename Iterator >
			struct virtual_impl_base_basic
				: public virtual_impl_interface< Self , Value , Category >
			{
			protected:
				Iterator i;
			public:
				virtual Self*  clone() const { return new Self(static_cast< const Self& >( *this )); }
				virtual Value& reference() const { return  *i; }
				virtual Value* pointer  () const { return &*i; }
				virtual bool   equals( const virtual_basic_impl_interface< Self , Value > & other ) {
					assert(typeid(other)==typeid(*this));
					return static_cast< const Self& >(other).i == this->i;
				}
			};
			
			template < typename Self , typename Value , typename Category , typename Iterator >
			struct virtual_impl_base_forward
				: public virtual_impl_base_basic< Self , Value , Category , Iterator >
			{
			protected:
				using virtual_impl_base_basic< Self , Value , Category , Iterator >::i;
			public:
				virtual void move_next() { ++i; }
			};

			template < typename Self , typename Value , typename Category , typename Iterator >
			struct virtual_impl_base_bidirectional
				: public virtual_impl_base_forward< Self , Value , Category , Iterator >
			{
			protected:
				using virtual_impl_base_forward< Self , Value , Category , Iterator >::i;
			public:
				virtual void move_prev() { --i; }
			};
			
			template < typename Self , typename Value , typename Category , typename Iterator >
			struct virtual_impl_base_random_access
				: public virtual_impl_base_bidirectional< Self , Value , Category , Iterator >
			{
			protected:
				using virtual_impl_base_bidirectional< Self , Value , Category , Iterator >::i;
			public:
				virtual void move_by( ptrdiff_t d ) { i += d; }
			};
			
			template < typename Self , typename Value , typename Category , typename Iterator > struct virtual_impl;
			
			template < typename Self , typename Value , typename Iterator >
			struct virtual_impl< Self , Value , std::forward_iterator_tag , Iterator >
			: public virtual_impl_base_forward< Self , Value , std::forward_iterator_tag , Iterator > {};
			
			template < typename Self , typename Value , typename Iterator >
			struct virtual_impl< Self , Value , std::bidirectional_iterator_tag , Iterator >
			: public virtual_impl_base_bidirectional< Self , Value , std::bidirectional_iterator_tag , Iterator > {};
			
			template < typename Self , typename Value , typename Iterator >
			struct virtual_impl< Self , Value , std::random_access_iterator_tag , Iterator >
			: public virtual_impl_base_random_access< Self , Value , std::random_access_iterator_tag , Iterator > {};
		}
	}
}

#endif //ndef IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL
