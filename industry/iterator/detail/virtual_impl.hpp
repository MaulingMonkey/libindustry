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
#include <cassert>
#include <cstddef>

#ifdef _DEBUG
#include <typeinfo>
#endif

namespace industry {
	namespace iterators {
		namespace detail {
			template < typename Self , typename Value , typename Category , typename Iterator >
			struct virtual_impl_base_basic
				: public virtual_impl_interface< Value , Category >
			{
			protected:
				Iterator i;
			public:
				virtual virtual_impl_interface< Value , Category >*  clone() const { return new Self(static_cast< const Self& >( *this )); }
				virtual Value& reference() const { return  *i; }
				virtual Value* pointer  () const { return &*i; }
				virtual bool   equals( const virtual_basic_impl_interface< virtual_impl_interface< Value , Category > , Value > & other ) {
					assert(typeid(other)==typeid(*this));
					return static_cast< const virtual_impl_base_basic<Self,Value,Category,Iterator> & >(other).i == this->i;
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
			
			template < typename Value , typename Category , typename Iterator > struct virtual_impl;
			
			#define SUPER virtual_impl_base_forward< virtual_impl< Value , std::forward_iterator_tag , Iterator > , Value , std::forward_iterator_tag , Iterator >
			template < typename Value , typename Iterator >
			struct virtual_impl< Value , std::forward_iterator_tag , Iterator > : public SUPER {
			protected:
				using SUPER::i;
			public:
				virtual_impl( const Iterator & new_i ) { i = new_i; }
			};
			#undef SUPER
			
			#define SUPER virtual_impl_base_bidirectional< virtual_impl< Value , std::bidirectional_iterator_tag , Iterator > , Value , std::bidirectional_iterator_tag , Iterator >
			template < typename Value , typename Iterator >
			struct virtual_impl< Value , std::bidirectional_iterator_tag , Iterator > : public SUPER {
			protected:
				using SUPER::i;
			public:
				virtual_impl( const Iterator & new_i ) { i = new_i; }
			};
			#undef SUPER
			
			#define SUPER virtual_impl_base_random_access< virtual_impl< Value , std::random_access_iterator_tag , Iterator > , Value , std::random_access_iterator_tag , Iterator >
			template < typename Value , typename Iterator >
			struct virtual_impl< Value , std::random_access_iterator_tag , Iterator >
			: public SUPER {
			protected:
				using SUPER::i;
			public:
				virtual_impl( const Iterator & new_i ) { i = new_i; }
			};
			#undef SUPER
		}
	}
}

#endif //ndef IG_INDUSTRY_ITERATOR_DETAIL_VIRTUAL_IMPL
