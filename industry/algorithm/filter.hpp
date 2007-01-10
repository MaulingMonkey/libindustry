// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
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

#if 0
			template < typename OutputIterator >
			void operator()( typename boost::call_traits< typename F::argument_type >::param_type argument , OutputIterator & out ) const {
				if ( predicate( argument ) ) *out++ = argument;
			}
#endif

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
			
			/***
			test.algorithm.cpp:31: error: no match for 'operator|' in
				'industry::algorithm::operator|(const RangeT&, const industry::algorithm::filter_processor<F>&)
					[with RangeT = industry::range<unsigned int*>
					, F = std::pointer_to_unary_function<unsigned int, bool>
					]
					(
						(
							(const industry::algorithm::filter_processor<std::pointer_to_unary_function<unsigned int, bool> >&)
							(
								(const industry::algorithm::filter_processor<std::pointer_to_unary_function<unsigned int, bool> >*)
								(& industry::algorithm::filter [with Result = bool, Arg1 = unsigned int](<unnamed>::is_odd))
							)
						)
					)
					|
					industry::algorithm::push_back
					[with ContainerT = std::vector<unsigned int, std::allocator<unsigned int> >]
					(
						(
							(std::vector<unsigned int, std::allocator<unsigned int> >&)
							(& results)
						)
					)'
../libindustry/industry.algorithm.push.hpp:24: note: candidates are: void industry::algorithm::operator|(RangeT&, const industry::algorithm::push_back_processor<ContainerT>&) [with RangeT = industry::range<boost::filter_iterator<std::pointer_to_unary_function<unsigned int, bool>, unsigned int*> >, ContainerT = std::vector<unsigned int, std::allocator<unsigned int> >]

			***/
		};

		template < typename Result , typename Arg1 >
		filter_processor< std::pointer_to_unary_function< Arg1 , Result > > filter( Result (*function)( Arg1 ) ) {
			return filter_processor< std::pointer_to_unary_function< Arg1 , Result > >( std::ptr_fun( function ) );
		}
	}
	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_FILTER
