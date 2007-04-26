// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_NULLPTR
#define IG_INDUSTRY_NULLPTR

namespace industry {
	const struct {
		template<typename ValueT> operator ValueT*() const { return 0; }
		template<typename ClassT, typename ValueT> operator ValueT ClassT::*() const { return 0; }
	private:
		void operator&() const;
	} nullptr = {};
}

#endif //IG_INDUSTRY_NULLPTR
