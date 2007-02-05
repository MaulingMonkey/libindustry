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

namespace industry {
	namespace algorithm {
		template < typename Self > class unbound_processor {};


		template < typename PredicateProcessor >
		class filter_processor : public unbound_processor< filter_processor< PredicateProcessor > > {
			PredicateProcessor predicate;
		public:
			filter_processor( const PredicateProcessor & predicate ): predicate(predicate) {}

			template < typename PreceedingActiveProcessor >
			class activate {
				typedef typename PredicateProcessor::template activate< PreceedingActiveProcessor > PredicateActiveProcessor;
				PredicateActiveProcessor predicate;
			public:
				activate( const PreceedingActiveProcessor & preceeding , const filter_processor & self ): predicate(preceeding,self.predicate) {}
				typedef typename PredicateActiveProcessor::result_type result_type;

				void        advance()   { while (!preceeding.end() && predicate.get()) preceeding.advance(); }
				bool        end() const { return preceeding.end(); }
				result_type get() const { return preceeding.get(); }
				
			};
		};

		template < typename PredicateProcessor >
		filter_processor< PredicateProcessor > filter( const PredicateProcessor & predicate ) {
			return filter_processor< PredicateProcessor >( predicate );
		}
	}
}

#endif //ndef IG_INDUSTRY_ALGORITHM_FILTER








#if 0 //Old header -- for reference only:
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
#endif
