// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Nov 11, 2006 - Created

#ifndef IG_INDUSTRY_ALGORITHM_FILTER
#define IG_INDUSTRY_ALGORITHM_FILTER

#include <industry/algorithm.hpp>
#include <industry/range.hpp>
#include <industry/traits/range.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/call_traits.hpp>
#include <functional>

namespace industry {
	namespace algorithm {
		template < typename F >
		class filter_processor {
			F predicate;
		public:
			filter_processor( F predicate ) : predicate( predicate ) {}

			template < typename RangeT >
			friend industry::range< boost::filter_iterator< F , typename range_traits< RangeT >::iterator > >
				operator|( RangeT & range_ , const filter_processor<F> & filt )
			{
				typedef boost::filter_iterator< F , typename range_traits< RangeT >::iterator > iterator_type;
				iterator_type begin( filt.predicate , range_traits<RangeT>::begin(range_) , range_traits<RangeT>::end(range_) );
				iterator_type end  ( filt.predicate , range_traits<RangeT>::end(range_)   , range_traits<RangeT>::end(range_) );
				return industry::range< iterator_type >( begin , end );
			}
			
			template < typename RangeT >
			friend industry::range< boost::filter_iterator< F , typename range_traits< const RangeT >::iterator > >
				operator|( const RangeT & range_ , const filter_processor & filt )
			{
				typedef boost::filter_iterator< F , typename range_traits< const RangeT >::iterator > iterator_type;
				iterator_type begin( filt.predicate , range_traits<RangeT>::begin(range_) , range_traits<RangeT>::end(range_) );
				iterator_type end  ( filt.predicate , range_traits<RangeT>::end(range_)   , range_traits<RangeT>::end(range_) );
				return industry::range< iterator_type >( begin , end );
			}
		};

		template < typename Result , typename Arg1 >
		filter_processor< std::pointer_to_unary_function< Arg1 , Result > > filter( Result (*function)( Arg1 ) ) {
			return filter_processor< std::pointer_to_unary_function< Arg1 , Result > >( std::ptr_fun( function ) );
		}
	}
	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_FILTER
