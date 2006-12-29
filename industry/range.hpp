// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.range.hpp => industry/range.hpp
// Nov 11, 2006 - Created

#ifndef IG_INDUSTRY_RANGE
#define IG_INDUSTRY_RANGE

#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>
#include <iterator>
#include <industry/traits/range.hpp>

namespace industry {
	template < typename IteratorT >
	class range {
		IteratorT begin_, end_;
		template < typename > friend class range;
	public:
		typedef IteratorT                                                        const_iterator;
		typedef IteratorT                                                        iterator;
		typedef typename std::iterator_traits< IteratorT >::difference_type      difference_type;
		typedef typename std::allocator<
			typename std::iterator_traits<iterator>::value_type
		>::size_type                                                             size_type;

		range()                                                  : begin_()               , end_()             {}
		range( const IteratorT & begin , const IteratorT & end ) : begin_( begin )        , end_( end )        {}
		range( const range & other )                             : begin_( other.begin_ ) , end_( other.end_ ) {}
		template < typename O >              range( const range<O>   & other     ) : begin_( other.begin_ )      , end_( other.end_ )      {}
		template < typename ContainerT >     range( ContainerT       & container ) : begin_( container.begin() ) , end_( container.end() ) {}
		template < typename T , unsigned N > range( T (&array)[N]                ) : begin_( array )             , end_( array + N )       {}

		IteratorT begin() const { return begin_; }
		IteratorT end()   const { return end_; }
		size_type size()  const { difference_type d = end_ - begin_; return (d > 0) ? size_type(+d) : size_type(-d); }
		bool      empty() const { return begin_ == end_; }

		template < typename OIt >
		friend bool operator==( const range & lhs , const range< OIt > & rhs ) {
			return (lhs.empty() && rhs.empty()) || (lhs.size() == rhs.size()) && std::equal( lhs.begin() , lhs.end() , rhs.begin() );
		}
		template < typename OIt >
		friend bool operator!=( const range & lhs , const range< OIt > & rhs ) {
			return !(lhs == rhs);
		}
	};

	template < typename IteratorT >
	range< IteratorT > make_range( const IteratorT & begin , const IteratorT & end ) {
		return range< IteratorT >( begin , end );
	}
	template < typename T >
	range< T * > make_range( T * begin , T * end ) {
		//allows arrays to correctly decay
		//(avoids attempting to create a range< T[N] >)
		//(avoids problems due to mismatching arrays and pointers (e.g. make_range(array,array+N)) )
		return range< T * >( begin , end );
	}
	template < typename T >
	range< const T * > make_range( const T * begin , const T * end ) {
		return range< const T * >( begin , end );
	}
	template < typename ContainerT >
	range< typename ContainerT::iterator > make_range( ContainerT & container ) {
		return range< typename ContainerT::iterator >( container );
	}
	
	template < typename ContainerT >
	range< typename ContainerT::const_iterator > make_range( const ContainerT & container ) {
		return range< typename ContainerT::const_iterator >( container );
	}

	template < typename T , unsigned N >
	range< T * > make_range( T (&array)[N] ) {
		return range< T * >( array );
	}
	template < typename T , unsigned N >
	range< const T * > make_range( const T (&array)[N] ) {
		return range< const T * >( array );
	}

	template < typename Iter1 , typename Iter2 >
	class multi_iterator;

	template< typename ItorT, typename RangeT >
	range< multi_iterator< ItorT, typename range_traits<RangeT>::iterator > >
		operator+(range<ItorT>& lhs, RangeT & rhs)
	{
		typedef multi_iterator< typename range<ItorT>::iterator, typename range_traits<RangeT>::iterator > iterator_type;
		return range< iterator_type >(iterator_type(lhs.begin(), lhs.end(), range_traits<RangeT>::begin(rhs), range_traits<RangeT>::end(rhs)), iterator_type());
	}

	template< typename ItorT, typename RangeT >
	range< multi_iterator< typename range<ItorT>::const_iterator, typename range_traits<RangeT>::const_iterator > >
		operator+(range<ItorT> const & lhs, RangeT const & rhs)
	{
		typedef multi_iterator< typename range<ItorT>::const_iterator, typename range_traits<RangeT>::const_iterator > iterator_type;
		return range< iterator_type >(iterator_type(lhs.begin(), lhs.end(), range_traits<RangeT>::begin(rhs), range_traits<RangeT>::end(rhs)), iterator_type());
	}

	template< typename RangeT1, typename RangeT2 >
	range< multi_iterator< typename range_traits<RangeT1>::iterator, typename range_traits<RangeT2>::iterator > >
		inorder(RangeT1 & lhs, RangeT2 & rhs)
	{
		typedef multi_iterator< typename range_traits<RangeT1>::iterator, typename range_traits<RangeT2>::iterator > iterator_type;
		return range< iterator_type >(iterator_type(range_traits<RangeT1>::begin(lhs), range_traits<RangeT1>::end(lhs), range_traits<RangeT2>::begin(rhs), range_traits<RangeT2>::end(rhs)), iterator_type());
	}

	template< typename RangeT1, typename RangeT2 >
	range< multi_iterator< typename range_traits<RangeT1>::const_iterator, typename range_traits<RangeT2>::const_iterator > >
	inorder(RangeT1 const & lhs, RangeT2 const & rhs)
	{
		typedef multi_iterator< typename range_traits<RangeT1>::const_iterator, typename range_traits<RangeT2>::const_iterator > iterator_type;
		return range< iterator_type >(iterator_type(range_traits<RangeT1>::begin(lhs), range_traits<RangeT1>::end(lhs), range_traits<RangeT2>::begin(rhs), range_traits<RangeT2>::end(rhs)), iterator_type());
	}
}

#endif //ndef IG_INDUSTRY_RANGE
