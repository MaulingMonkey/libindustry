// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  1, 2007 - Created

namespace industry {
	namespace templates {
		template < template < typename > class class_ > struct tt1 { template < typename T1 > struct with { typedef class_< T1 > type; }; };
	}
	using ::industry::templates::tt1;
}
