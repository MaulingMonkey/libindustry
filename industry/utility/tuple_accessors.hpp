// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS
#define IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS

#include <industry/sfinae.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace industry {
	namespace utility {
		// Implementation roster:
		// T get_at_type           < T >   ( Tuple )
		// T get_at_type_or_default< T >   ( Tuple , default )
		// T get_if_type           < Pred >( Tuple )
		// T get_if_type_or_default< Pred >( Tuple , default )
	}
}

#endif //ndef IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS
