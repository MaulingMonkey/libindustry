// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_CONCEPTS
#define IG_INDUSTRY_ALGORITHM_CONCEPTS

#if 1

#else 0 //Old header -- for reference only:
#include <industry/range.hpp>

namespace industry {
	namespace algorithm {
		template < typename BoundProcessor > struct active_processor_of { typedef typename BoundProcessor::active_processor_type type; };
		template < typename Iterator > class active_range_processor {
			      Iterator i;
			const Iterator end_i;
		public:
			typedef typename industry::range<Iterator>::reference reference;
			active_range_processor( industry::range<Iterator> const& r ): i(r.begin()), end_i(r.end()) {}

			reference get() const { return *i;} 
			void advance() { ++i; }
			bool end() { return i == end_i; }
		};
		template < typename Iterator > struct active_processor_of< industry::range< Iterator > > { typedef active_range_processor<Iterator> type; };

		//template < typename ActiveProccessorT , typename NextUnboundProcessor = industry::nil > struct unbound_processor {
		//	friend ... activate( unbound_processor const& p ) {
		//};

		//template < typename BoundProcessorConvertable > struct bound_processor_of {};
		/* Primary concepts:
		 *
		 * - UnboundProcessor
		 *   A processor not bound to an input range -- used for secondary processor inputs
		 *   UnboundProcessor | UnboundProcessor results in another UnboundProcessor (a "chained" processor)
		 *   Example:  Result of "filter", or "transform"
		 *
		 * - BoundProcessor
		 *   A processor bound to an input range
		 *   Example:  Result of "each"
		 *
		 * - ActiveProcessor
		 *   A processor with iteration state data -- invokation of a BoundProcessor chain -- actual processor logic here.
		 *   Interface:
		 *      reference get() const; //current element
		 *      void advance(); //advance to next position
		 *      bool end(); //get() no longer valid
		 *
		 * - EndpointProcessor
		 *   A processor which when concatonated to a BoundProcessor chain invokes an operation upon the iterated over elements.
		 *   Takes a BoundProcessor, converts to an ActiveProcessor, and uses said chain.
		 *
		 *   Examples:  Result of "call", or "push_back"
		 *
		 * Other concepts that map to algorithm concepts:
		 *
		 * - Range      is-a        BoundProcessor
		 * - Container  converts-to Range             is-a BoundProcessor
		 *
		 * Other concepts that map to specific algorithm items:
		 *
		 * - Function Pointers         convert-to UnboundProcessor[ transform_fun ]
		 * - Member Function Pointers  convert-to UnboundProcessor[ transform_mem_fun ]
		 * - Unary Functors            convert-to UnboundProcessor[ transform_functor ]
		 * - 
		 */
	}
}
#endif

#endif //ndef IG_INDUSTRY_ALGORITHM_CONCEPTS
