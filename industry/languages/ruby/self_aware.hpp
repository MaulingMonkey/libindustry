// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_SELF_AWARE
#define IG_INDUSTRY_LANGUAGES_RUBY_SELF_AWARE

#include <industry/languages/ruby/detail/self_aware_base.hpp>
#include <industry/languages/ruby/value.hpp>
#include <string>

namespace industry { namespace languages { namespace ruby {
	template < typename T > class self_aware : public detail::self_aware_base<T> {
	public:
		value self() { return value(raw_self()); }
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_SELF_AWARE
