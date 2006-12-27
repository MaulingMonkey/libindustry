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
#include <limits>

namespace industry {
	template < typename IteratorT >
	class range {
		IteratorT begin_, end_;
		template < typename > friend class range;
	public:
		typedef IteratorT                                                        const_iterator;
		typedef IteratorT                                                        iterator;
		typedef typename std::iterator_traits< IteratorT >::difference_type      difference_type;
	private:
		//Ugly hack since iterator_traits dosn't give us a nice size_type:
		typedef typename std::iterator_traits< IteratorT >::difference_type      size_type_;
		BOOST_STATIC_ASSERT(( !std::numeric_limits< size_type_ >::is_signed
		                   || sizeof(unsigned) >= sizeof(size_type_)
		                   ));
	public:
		typedef typename boost::mpl::if_c< std::numeric_limits< size_type_ >::is_signed , unsigned , size_type_ >::type size_type;

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
}

#endif //ndef IG_INDUSTRY_RANGE
