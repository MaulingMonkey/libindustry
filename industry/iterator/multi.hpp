// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 27, 2006 - Created
//
// NOTE:  Only forward, bidirectional, and random access iterators are compatible currently.

#ifndef IG_INDUSTRY_ITERATOR_MULTI
#define IG_INDUSTRY_ITERATOR_MULTI

#include <industry/traits/iterator_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <iterator>
#include <cassert>

namespace industry {
	namespace detail {
		template < typename Iter1Tag , typename Iter2Tag >
		struct multi_iterator_category_traits;
		
		template < typename Iter2Tag >
		struct multi_iterator_category_traits< std::forward_iterator_tag , Iter2Tag > {
			typedef std::forward_iterator_tag iterator_category;
		};
		template <>
		struct multi_iterator_category_traits< std::bidirectional_iterator_tag , std::forward_iterator_tag > {
			typedef std::forward_iterator_tag iterator_category;
		};
		template < typename Iter2Tag >
		struct multi_iterator_category_traits< std::bidirectional_iterator_tag , Iter2Tag > {
			typedef std::bidirectional_iterator_tag iterator_category;
		};
		template < typename Iter2Tag >
		struct multi_iterator_category_traits< std::random_access_iterator_tag , Iter2Tag > {
			typedef Iter2Tag iterator_category;
		};
		
		template < typename ValueT , typename Iter1Result , typename Iter2Result >
		struct multi_iterator_result_traits;
		
		template < typename Iter1 , typename Iter2 >
		struct multi_iterator_traits
			: multi_iterator_category_traits
				< typename std::iterator_traits< Iter1 >::iterator_category
				, typename std::iterator_traits< Iter2 >::iterator_category
				>
		{
			BOOST_STATIC_ASSERT(( boost::is_same< typename std::iterator_traits< Iter1 >::value_type , typename std::iterator_traits< Iter2 >::value_type >::value ));
			typedef typename std::iterator_traits< Iter1 >::value_type value_type;
			typedef typename boost::mpl::if_c< sizeof(typename std::iterator_traits< Iter1 >::difference_type) >= sizeof(typename std::iterator_traits< Iter2 >::difference_type)
			                                 , typename std::iterator_traits< Iter1 >::difference_type
			                                 , typename std::iterator_traits< Iter2 >::difference_type
			                                 >::type           difference_type;
			typedef value_type* pointer;
			typedef value_type& reference;
		};
		
	}
	template < typename Iter1 , typename Iter2 >
	class multi_iterator : industry::detail::multi_iterator_traits< Iter1 , Iter2 >
	{
		typedef multi_iterator< Iter1 , Iter2 > this_t;
	protected:
		Iter1 begin1, i1, end1;
		Iter2 begin2, i2, end2;
		unsigned iter_set; //0 == end, 1 == set 1, 2 == set 2, * == invalid
	public:
		typedef typename industry::detail::multi_iterator_traits< Iter1 , Iter2 >::iterator_category iterator_category;
		typedef typename industry::detail::multi_iterator_traits< Iter1 , Iter2 >::difference_type   difference_type;
		typedef typename industry::detail::multi_iterator_traits< Iter1 , Iter2 >::value_type        value_type;
		typedef typename industry::detail::multi_iterator_traits< Iter1 , Iter2 >::pointer           pointer;
		typedef typename industry::detail::multi_iterator_traits< Iter1 , Iter2 >::reference         reference;
						
		multi_iterator(): begin1(), i1(), end1(), begin2(), i2(), end2(), iter_set(0)
		{
			//Immutable end case
		}
		multi_iterator( const this_t & other
					  ): begin1(other.begin1), i1(other.i1), end1(other.end1)
					  ,  begin2(other.begin2), i2(other.i2), end2(other.end2), iter_set(other.iter_set)
		{
			//Copy case
		}
		multi_iterator( Iter1 begin1 , Iter1 end1, Iter2 begin2 , Iter2 end2
					  ): begin1(begin1), i1(begin1), end1(end1)
					  ,  begin2(begin2), i2(begin2), end2(end2), iter_set(1)
		{
			//Begin iterator
			
			if ( begin1 == end1 ) iter_set = 2;
			//if begin2 == end2 as well, iterset == 2 still
		}
		this_t & operator++() {
			assert( iter_set || !"Tried to increment an immutable end iterator" );
			
			if ( iter_set == 1 ) {
				assert( i1 != end1 || !"Should never happen" ); //iter_set should be 2 if i1==end1
				++i1;
				if ( i1 == end1 ) {
					iter_set = 2;
				}
			} else if ( iter_set == 2 ) {
				assert( i2 != end2 || !"Tried to increment past end" );
				++i2;
			} else {
				assert( !"Should never happen" ); //iter_set should only be 0, 1, or 2
			}
			return *this;
		}
		this_t & operator--() {
			assert( iter_set || !"Tried to decrement an immutable end iterator" );
			
			if ( iter_set == 2 ) {
				if ( begin2 == i2 ) {
					iter_set = 1; //will continue in iter_set == 1
				} else {
					--i2;
					return *this;
				}
			}
			
			if ( iter_set == 1 ) {
				assert( begin1 != i1 || !"Tried to decrement past begin" );
				--i1;
			}
			
			return *this;
		}
		this_t operator++(int) { this_t copy(*this); ++(*this); return copy; }
		this_t operator--(int) { this_t copy(*this); --(*this); return copy; }
		this_t & operator+=( difference_type difference ) {
			assert( iter_set || !"Tried to increment an immutable end iterator" );
			
			if ( difference < 0 ) return (*this) -= -difference;
			
			if ( iter_set == 1 ) {
				difference_type d1 = end1 - i1;
				if ( difference > d1 ) {
					i1 = end1;
					iter_set = 2; //will continue in iter_set == 2
					difference -= d1;
				} else {
					i1 += difference;
				}
			}
			
			if ( iter_set == 2 ) {
				difference_type d2 = end2 - i2;
				assert( difference <= d2 || !"Tried to increment past end" );
				i2 += difference;
			}
			
			return *this;
		}
		this_t & operator-=( difference_type difference ) {
			assert( iter_set || !"Tried to decrement an immutable end iterator" );
			
			if ( difference < 0 ) return (*this) += -difference;
			
			if ( iter_set == 2 ) {
				difference_type d2 = i2 - begin2;
				if ( difference > d2 ) {
					i2 = begin2;
					iter_set = 1;
					difference -= d2;
				} else {
					i2 -= difference;
				}
			}
			
			if ( iter_set == 1 ) {
				difference_type d1 = i1 - begin1;
				assert( difference <= d1 || !"Tried to decrement past end" );
				i1 -= difference;
			}
			
			return *this;
		}
		this_t operator+( difference_type difference ) { this_t copy(*this); copy += difference; return copy; }
		this_t operator-( difference_type difference ) { this_t copy(*this); copy -= difference; return copy; }
	private:
		static void assert_same_range( const this_t & lhs , const this_t & rhs ) {
			assert( lhs.begin1 == rhs.begin1 );
			assert( lhs.end1   == rhs.end1   );
			assert( lhs.begin2 == rhs.begin2 );
			assert( lhs.end2   == rhs.end2   );
		}
		static bool is_at_end( const this_t & t ) {
			return (t.iter_set == 0)
			    || (t.iter_set == 2 && t.i2 == t.end2)
				;
		}
	public:
		friend bool operator==( const this_t & lhs , const this_t & rhs ) {
			if ( lhs.iter_set && rhs.iter_set ) {
				assert_same_range( lhs , rhs );
				if ( lhs.iter_set != rhs.iter_set ) return false;
				switch ( lhs.iter_set ) {
					case 1: return lhs.i1 == rhs.i1;
					case 2: return lhs.i2 == rhs.i2;
					default: assert(!"Should never happen" );
				}
			}
		
			return is_at_end( lhs ) == is_at_end( rhs );
		}
		
		friend bool operator!=( const this_t & lhs , const this_t & rhs ) {
			return !(lhs==rhs);
		}
		friend bool operator<=( const this_t & lhs , const this_t & rhs ) {
			if (!rhs.iter_set) return true;
			else if (!lhs.iter_set) return is_at_end( rhs );
			
			assert_same_range( lhs , rhs );
			
			if (lhs.iter_set != rhs.iter_set) return lhs.iter_set < rhs.iter_set;
			else switch ( lhs.iter_set ) {
				case 1: return lhs.i1 <= rhs.i1;
				case 2: return lhs.i2 <= rhs.i2;
				default: assert(!"Should never happen");
			}
		}
		friend bool operator>=( const this_t & lhs , const this_t & rhs ) {
			return rhs <= lhs;
		}
		friend bool operator< ( const this_t & lhs , const this_t & rhs ) {
			if (!rhs.iter_set) return !is_at_end( rhs );
			else if (!lhs.iter_set) return false;
			
			assert_same_range( lhs , rhs );
			
			if (lhs.iter_set != rhs.iter_set) return lhs.iter_set < rhs.iter_set;
			else switch ( lhs.iter_set ) {
				case 1: return lhs.i1 < rhs.i1;
				case 2: return lhs.i2 < rhs.i2;
				default: assert(!"Should never happen");
			}
		}
		friend bool operator> ( const this_t & lhs , const this_t & rhs ) {
			return rhs < lhs;
		}
		reference operator*() const {
			switch (iter_set) {
				case 1:
					return *i1;
				case 2:
					assert( i2 != end2 || !"Tried to dereference end" );
					return *i2;
				case 0:  assert(!"Tried to derefernce immutable end iterator!");
				default: assert(!"Should never happen"); throw;
			}
		}
		pointer operator->() const {
			switch(iter_set) {
				case 1:  return &*i1;
				case 2:  assert( i2 != end2 || !"Tried to dereference end" ); return &*i2;
				case 0:  assert(!"Tried to dereference immutable end iterator!");
				default: assert(!"Should never happen");
			}
		}
		reference operator[]( difference_type difference ) const { return *((*this)+difference); }
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_MULTI
