// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_CONCEPTS
#define IG_INDUSTRY_ALGORITHM_CONCEPTS

namespace industry {
	namespace algorithm {
		/* Primary concepts:
		 *
		 * - UnboundProcessor
		 *   A processor not bound to an input range -- used for secondary processor inputs
		 *   UnboundProcessor | UnboundProcessor results in another UnboundProcessor (a "chained" processor)
		 *   Examples:  filter, transform
		 *
		 * - BoundProcessor
		 *   A processor bound to an input range
		 *   Examples:  each
		 *
		 * - EndpointProcessor
		 *   A processor which when concatonated to a BoundProcessor chain invokes an operation upon the iterated over elements
		 *   Examples:  call, push_back
		 *
		 * Other concepts that map to algorithm concepts:
		 *
		 * - Range      is-a        UnboundProcessor
		 * - Container  converts-to Range             is-a UnboundProcessor
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

#endif //ndef IG_INDUSTRY_ALGORITHM_CONCEPTS
