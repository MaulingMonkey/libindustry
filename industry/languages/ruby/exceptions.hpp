// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_EXCEPTIONS
#define IG_INDUSTRY_LANGUAGES_RUBY_EXCEPTIONS

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <boost/format.hpp>
#include <stdexcept>

namespace industry { namespace languages { namespace ruby {
	class ruby_error : public std::runtime_error {
	public:
		ruby_error(std::string const& message): std::runtime_error(message) {}
	};

	class ruby_type_error : public ruby_error {
	public:
		ruby_type_error( const char* description, VALUE klass )
			: ruby_error(str(boost::format(description)%rb_class2name(klass)))
		{
		}
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_EXCEPTIONS
