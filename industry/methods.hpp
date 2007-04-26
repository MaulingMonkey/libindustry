// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 26, 2006 - industry.methods.hpp => industry/methods.hpp
// May 21, 2006 - Created

#ifndef IG_INDUSTRY_METHODS
#define IG_INDUSTRY_METHODS

#include <industry/config.hpp>
#include <industry/multitype.hpp>
#include <industry/nil.hpp>
#include <boost/preprocessor/repetition.hpp>

#ifdef INDUSTRY_METHODS_LIMIT
#error "Cannot override INDUSTRY_METHODS_LIMIT, currently required to be INDUSTRY_MULTITYPE_LIMIT"
#else //ndef INDUSTRY_METHODS_LIMIT
#define INDUSTRY_METHODS_LIMIT INDUSTRY_MULTITYPE_LIMIT
#endif //ndef INDUSTRY_METHODS_LIMIT

namespace industry {
	template < BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_METHODS_LIMIT , typename M , nil ) > struct methods {};
}

#endif //ndef IG_INDUSTRY_METHODS
