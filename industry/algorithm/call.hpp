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
		namespace detail {
			template<typename F>
			struct function_traits { };

			template<typename R, typename A1>
			struct function_traits<R (*)(A1)> {
				typedef R result_type;
				typedef A1 argument_type;

				typedef R (*function_pointer)(A1);
			};

			template<typename R, typename A1>
			struct function_traits<R (A1)> {
				typedef R result_type;
				typedef A1 argument_type;

				typedef R (*function_pointer)(A1);
			};

			template<typename R, typename C>
			struct function_traits<R (C::*)()> {
				typedef R result_type;
				typedef C& argument_type;

				typedef R (C::*function_pointer)();
			};
		}

		template < typename FunctionS >
		class call_processor {
			typedef typename detail::function_traits<FunctionS>::result_type result_type;
			typedef typename detail::function_traits<FunctionS>::argument_type argument_type;

			typedef boost::function< result_type ( argument_type )> function_type;
			function_type function;
		public:
			call_processor( function_type function ) : function( function ) { }

			template < typename RangeT >
			friend void operator|( RangeT & range , const call_processor< FunctionS > & p ) {
				std::for_each(range_traits<RangeT>::begin(range), range_traits<RangeT>::end(range), p);
			}

			template < typename RangeT >
			friend void operator|( const RangeT & range , const call_processor< FunctionS > & p ) {
				std::for_each(range_traits<RangeT>::begin(range), range_traits<RangeT>::end(range), p);
			}

			typename detail::function_traits<FunctionS>::result_type operator()( typename detail::function_traits<FunctionS>::argument_type argument ) {
				return function(argument);
			}
		};

		template < typename FunctionS >
		call_processor< FunctionS > call( FunctionS function ) {
			return call_processor< FunctionS >( function );
		}
	}
	using namespace algorithm;
}

#endif //ndef IG_INDUSTRY_ALGORITHM_PUSH
