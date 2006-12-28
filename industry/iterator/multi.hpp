// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 27, 2006 - Created
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ITERATOR_MULTI
#define IG_INDUSTRY_ITERATOR_MULTI

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <iterator>

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
		
		template < typename Iter1 , typename Iter2 >
		struct multi_iterator_traits
			: multi_iterator_category_traits
				< typename std::iterator_traits< Iter1 >::iterator_category
				, typename std::iterator_traits< Iter2 >::iterator_category
				>
		{
			BOOST_STATIC_ASSERT(( boost::is_same< typename Iter1::value_type , typename Iter2::value_type >::value ));
			typedef typename Iter1::value_type value_type;
			typedef typename boost::mpl::if_c< sizeof(typename Iter1::difference_type) >= sizeof(typename Iter2::difference_type)
			                                 , typename Iter1::difference_type
			                                 , typename Iter2::difference_type
			                                 >::type           difference_type;
			typedef value_type* pointer;
			typedef value_type& reference;
		};
		
	}
	template < typename Iter1 , typename Iter2 >
	class multi_iterator
	{
		typedef multi_iterator< Iter1 , Iter2 > this_t;
		
		Iter1 begin1, i1, end1;
		Iter2 begin2, i2, end2;
		unsigned iter_set; //0 == end, 1 == set 1
	public:
		typedef typename detail::multi_iterator_traits< Iter1 , Iter2 >::iterator_category iterator_category;
		typedef typename detail::multi_iterator_traits< Iter1 , Iter2 >::difference_type   difference_type;
		typedef typename detail::multi_iterator_traits< Iter1 , Iter2 >::value_type        value_type;
		typedef typename detail::multi_iterator_traits< Iter1 , Iter2 >::pointer           pointer;
		typedef typename detail::multi_iterator_traits< Iter1 , Iter2 >::reference         reference;
		
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
			assert( iter_set != 2 || i2 != end2 || !"Tried to increment past the end" );
		}
		this_t & operator--() {
			assert( iter_set || !"Tried to decrement an immutable end iterator" );
			
			if ( iter_set == 2 ) {
				if ( begin2 == i2 ) {
					iter_set = 1;
					//will continue in iter_set == 1
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
				if ( difference >= d1 ) {
					i1 = end1;
					iter_set = 2;
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
				if ( difference >= d2 ) {
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
	};
}

#endif //ndef IG_INDUSTRY_ITERATOR_MULTI
