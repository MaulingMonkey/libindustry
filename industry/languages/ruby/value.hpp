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
#include <industry/languages/ruby/detail/ruby_value.hpp>
#include <boost/preprocessor.hpp>

namespace industry { namespace languages { namespace ruby {
	class value;
	class lazy_value {
		friend class value;
		VALUE value_;

		lazy_value();
		lazy_value( const lazy_value& );
	public:
		lazy_value( VALUE v ): value_(v) {}

		template < typename T > T to() const { return detail::ruby_value<T>::from(value_); }
	};

	class value_function {
		VALUE self;
		ID    id;
	public:
		value_function( VALUE, const char* str );
		value_function( VALUE, const char* str, unsigned strlen );
		~value_function();

		lazy_value operator()() const {
			return rb_funcall3( self, id, 0, NULL );
		}

#		define OPERATOR_CALL_CONVERSION(z,n,arg) detail::ruby_value< A ## n >::to( arg ## n )
#		define OPERATOR_CALL_IMPL( z, n, unused )                                   \
		template < BOOST_PP_ENUM_PARAMS(n,typename A) >                             \
		lazy_value operator()( BOOST_PP_ENUM_BINARY_PARAMS( n, A, arg ) ) const {   \
			VALUE values[] = { BOOST_PP_ENUM( n, OPERATOR_CALL_CONVERSION, arg ) }; \
			return rb_funcall3( self, id, n, values );                              \
		} /*-----------------------------------------------------------------------*/
		BOOST_PP_REPEAT_FROM_TO(1,11,OPERATOR_CALL_IMPL,~);
#		undef OPERATOR_CALL_IMPL
#		undef OPERATOR_CALL_CONVERSIONS

	};

	class value {
		VALUE value_;
	public:
		value();
		value( const value     & );
		value( const lazy_value& );
		value( VALUE );
		~value();


		value_function operator->*( const char *       str ) const { return value_function(value_,str); }
		value_function operator->*( const std::string& str ) const { return value_function(value_,str.c_str(),str.size()); }
		template < size_t N >
		value_function operator->*( const char (&str)[N]   ) const { return value_function(value_,str,N-1); }

		template < typename T > T to() const { return detail::ruby_value<T>::from(value_); }
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_VALUE
