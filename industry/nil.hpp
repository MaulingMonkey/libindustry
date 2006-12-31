// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 31, 2006 - Added templateN_nil
// Dec 25, 2006 - industry.nil.hpp => industry/nil.hpp
// May 20, 2006 - Created

// TODO:
// - Add preprocessor generation and N max #define control

#ifndef IG_INDUSTRY_NIL
#define IG_INDUSTRY_NIL

namespace industry {
	struct nil {};
	
	template < typename > struct template1_nil {};
	template < typename , typename > struct template2_nil {};
	template < typename , typename , typename > struct template3_nil {};
	template < typename , typename , typename , typename > struct template4_nil {};
	template < typename , typename , typename , typename , typename > struct template5_nil {};
	template < typename , typename , typename , typename , typename , typename > struct template6_nil {};
	template < typename , typename , typename , typename , typename , typename , typename > struct template7_nil {};
	template < typename , typename , typename , typename , typename , typename , typename , typename > struct template8_nil {};
	template < typename , typename , typename , typename , typename , typename , typename , typename , typename > struct template9_nil {};
	template < typename , typename , typename , typename , typename , typename , typename , typename , typename , typename > struct template10_nil {};
}

#endif //ndef IG_INDUSTRY_NIL
