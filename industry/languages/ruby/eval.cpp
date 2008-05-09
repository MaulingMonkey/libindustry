// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/languages/ruby/eval.hpp>
#include <sstream>
#include <numeric>

namespace industry { namespace languages { namespace ruby {
	lazy_value eval( const std::string& str ) {
		return rb_eval_string( str.c_str() );
	}
	lazy_value eval( const char* str ) {
		return rb_eval_string( str );
	}

	lazy_value safe_eval( const std::string& str ) {
		return safe_eval(str.c_str());
	}

	lazy_value safe_eval( const char* str ) {
		struct protector {
			static VALUE body(VALUE arg) {
				std::string s = detail::ruby_value<std::string>::from(arg);
				s = std::accumulate(s.begin(), s.end(), std::string(), protector::indent);
				std::string evl = "begin\n\t" + s + "\nrescue\n\t$__err__=$!\nraise\nend\nx";
				return rb_eval_string(evl.c_str());
			}

		private:
			static std::string indent(std::string const& was, char c) {
				if(c == '\n')
					return was + "\n\t";
				return was + c;
			}
		};

		int error;
		VALUE result = rb_protect(protector::body, detail::ruby_value<std::string>::to(str), &error);
		if(error) {
			std::ostringstream errorText;
			value backtrace = eval("$__err__");

			errorText<<detail::ruby_value<std::string>::from(rb_obj_as_string(backtrace.get_value()))<<std::endl;

			if(backtrace) {
				value ary = (backtrace ->* "backtrace")();
				for(std::size_t i = 0; i < ary.length(); ++i) {
					errorText<<"\tfrom "<<ary[i].to<std::string>()<<std::endl;
				}
			}

			throw ruby_error(errorText.str());
		}

		return result;
	}
}}}
