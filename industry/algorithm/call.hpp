// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_CALL
#define IG_INDUSTRY_ALGORITHM_CALL

#include <algorithm>
#include <iterator>
#include <industry/traits/range.hpp>
#include <boost/function.hpp>
#include <boost/functional.hpp>

namespace industry {
	namespace algorithm {
		template < typename FunctionS >
		class call_processor {
			typedef FunctionS function_type;
			function_type function;
		public:
			call_processor( function_type function ) : function( function ) { }

			template < typename RangeT >
			friend void operator|( RangeT & range , const call_processor< FunctionS > & p ) {
				std::for_each(range_traits<RangeT>::begin(range), range_traits<RangeT>::end(range), p.function);
			}

			template < typename RangeT >
			friend void operator|( const RangeT & range , const call_processor< FunctionS > & p ) {
				std::for_each(range_traits<RangeT>::begin(range), range_traits<RangeT>::end(range), p.function);
			}
		};

		template < typename R, typename A1 >
		call_processor< boost::function<R (A1)> > call( R (*function)(A1) ) {
			return call(boost::function<R (A1)>(function));
		}

		template < typename R, typename C >
		call_processor< boost::function<R (C&)> > call( R (C::*function)() ) {
			return call(boost::function<R (C&)>(function));
		}

		template< typename R, typename A1 >
		call_processor< boost::function<R (A1)> > call( boost::function<R (A1)> function) {
			return call_processor< boost::function<R (A1)> >(function);
		}
	}
	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_CALL
