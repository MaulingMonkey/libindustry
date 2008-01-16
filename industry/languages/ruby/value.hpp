// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_VALUE
#define IG_INDUSTRY_LANGUAGES_RUBY_VALUE

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>

namespace industry { namespace languages { namespace ruby {
	class value;
	class lazy_value {
		friend class value;
		VALUE value_;

		lazy_value();
		lazy_value( const lazy_value& );
	public:
		lazy_value( VALUE v ): value_(v) {}
	};
	class value {
		VALUE value_;
	public:
		value();
		value( const value     & );
		value( const lazy_value& );
		value( VALUE );
		~value();
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_VALUE
