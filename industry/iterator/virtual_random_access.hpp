// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  7, 2007 - Created

#ifndef IG_INDUSTRY_ITERATOR_VIRTUAL_RANDOM_ACCESS
#define IG_INDUSTRY_ITERATOR_VIRTUAL_RANDOM_ACCESS

#include <industry/iterator/virtual_bidirectional.hpp>
#include <boost/scoped_ptr.hpp>
#include <iterator>
#include <new>
#include <cassert>

namespace industry {
	namespace iterators {
		namespace detail {
			template < typename Self , typename Value , typename Interface >
			class random_access_base : public bidirectional_base< Self , Value , Interface > {
				Self & self()             { return static_cast<       Self& >( *this ); }
				const Self & self() const { return static_cast< const Self& >( *this ); }
				typedef Interface            iterator_interface_type;
				typedef bidirectional_base< Self , Value , Interface > super;
			protected:
				using super::i;
			public:
				typedef typename super::reference  reference;
				typedef typename super::pointer    pointer;
				
				random_access_base(): super() {}
				random_access_base( const random_access_base & copy ): super( static_cast< const super& >( copy ) ) {}
				
				template < typename RandomAccessIterator >
				explicit random_access_base( const RandomAccessIterator & iter ): super(iter) {}
				
				Self  & operator+=(ptrdiff_t d) { assert(i); i->move_by(d); return self(); }
				Self    operator+ (ptrdiff_t d) { assert(i); Self copy(self()); copy += d; return copy; }
				Self  & operator-=(ptrdiff_t d) { return *this += -d; }
				Self    operator- (ptrdiff_t d) { return *this +  -d; }
				Value & operator[](ptrdiff_t d) { assert(i); Self copy(self()); copy += d; return *copy; }
				friend  ptrdiff_t operator-( const random_access_base & lhs , const random_access_base & rhs ) { return rhs.i->distance_to( *lhs.i ); }
				friend  bool      operator<( const random_access_base & lhs , const random_access_base & rhs ) { return lhs - rhs < 0; }
				friend  bool      operator>( const random_access_base & lhs , const random_access_base & rhs ) { return lhs - rhs > 0; }
			};
		}
		#define SELF      virtual_random_access_iterator< Value >
		#define INTERFACE ::industry::iterators::detail::virtual_impl_interface< Value , std::random_access_iterator_tag >
		#define SUPER     ::industry::iterators::detail::random_access_base< SELF , Value , INTERFACE >
		template < typename Value >
		class virtual_random_access_iterator : public SUPER {
			typedef SELF  self;
			typedef SUPER super;
		#undef SELF
		#undef INTERFACE
		#undef SUPER
		public:
			typedef std::random_access_iterator_tag        iterator_category;
			typedef Value                                  value_type;
			
			virtual_random_access_iterator(): super() {}
			virtual_random_access_iterator( const virtual_random_access_iterator & copy ): super( static_cast< const super & >(copy) ) {}
			template < typename RandomAccessIterator > explicit virtual_random_access_iterator( const RandomAccessIterator & iter ): super(iter) {}
		};
	}
	using ::industry::iterators::virtual_random_access_iterator;
}

#endif //ndef IG_INDUSTRY_ITERATOR_VIRTUAL_RANDOM_ACCESS
