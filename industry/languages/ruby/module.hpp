// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_MODULE
#define IG_INDUSTRY_LANGUAGES_RUBY_MODULE

namespace industry {
	namespace languages {
		namespace ruby {
			class module {
				VALUE value;
			public:
				module( const std::string& name ) { value = rb_define_module(name.c_str()); }
				module( const module& parent, const std::string& name ) { value = rb_define_module_under(parent.value,name.c_str()); }

				VALUE get_value() const { return value; }
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_MODULE
