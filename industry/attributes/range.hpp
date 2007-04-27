// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 11, 2007 - Created

#ifndef IG_INDUSTRY_ATTRIBUTES_RANGE
#define IG_INDUSTRY_ATTRIBUTES_RANGE

#include <industry/traits/range.hpp>
#include <boost/range.hpp>

namespace industry {
	namespace attributes {
		namespace range {
			using ::boost::begin;
			using ::boost::end;
			using ::boost::size;
			using ::boost::empty;
		}
		namespace all {
			using namespace attributes::range;
		}
	}
}

#endif //ndef IG_INDUSTRY_ATTRIBUTES_RANGE
