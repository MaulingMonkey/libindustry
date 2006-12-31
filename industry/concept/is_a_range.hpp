// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.concept.is_a_range.hpp => industry/concept/is_a_range.hpp
// Nov 12, 2006 - Created

#ifndef IG_INDUSTRY_CONCEPT_IS_A_RANGE
#define IG_INDUSTRY_CONCEPT_IS_A_RANGE

namespace industry {
	namespace concept {
		template < typename T >
		struct is_a_range {
			typedef char[1] true_t;
			typedef char[2] false_t;

			template < typename IteratorT > true_t check( IteratorT (T::*)() = & T::begin , IteratorT (T::*)() = & T::end );
			false_t check( ... );

			static const bool value = (sizeof(check()) == sizeof(true_t));
		};
	}
}

#endif //ndef IG_INDUSTRY_CONCEPT_IS_A_RANGE
