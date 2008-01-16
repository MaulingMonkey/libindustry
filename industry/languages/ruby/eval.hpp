// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_EVAL
#define IG_INDUSTRY_LANGUAGES_RUBY_EVAL

#include <industry/languages/ruby/value.hpp>
#include <string>

namespace industry { namespace languages { namespace ruby {
	lazy_value eval( const char *       str );
	lazy_value eval( const std::string& str );

	template < typename T > T eval( const char*        str ) { return eval(str).to<T>(); }
	template < typename T > T eval( const std::string& str ) { return eval(str).to<T>(); }
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_EVAL
