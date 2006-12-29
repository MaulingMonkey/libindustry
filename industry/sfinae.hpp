// Copyright (c) 2006 Sean M. Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_SFINAE
#define IG_INDUSTRY_SFINAE

#include <algorithm>
#include <iterator>
#include <industry/traits/range.hpp>
#include <boost/function.hpp>
#include <boost/functional.hpp>

namespace industry {
	template<typename T>
	struct wrap {};

	struct sfinae {
		typedef char one;
		typedef struct { char c[2]; } two;
	};
}

#endif //ndef IG_INDUSTRY_SFINAE
