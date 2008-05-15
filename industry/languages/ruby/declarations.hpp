// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy: washuhibiki $ - $LastChangedDate: 2008-05-13 13:45:12 -0700 (Tue, 13 May 2008) $

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_DECLARATIONS
#define IG_INDUSTRY_LANGUAGES_RUBY_DECLARATIONS

namespace industry { namespace languages { namespace ruby {
	template < typename T, typename B = void > struct class_;
	
	namespace detail {
		template < typename T > struct ruby_value;
	}
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_DECLARATIONS
