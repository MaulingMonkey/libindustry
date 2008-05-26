// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS
#define IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/range.hpp>
#include <map>
#include <string>
#include <cassert>

namespace industry { namespace languages { namespace ruby { namespace detail {
	template < typename T > struct class_registry;
	template < typename T > struct ruby_value;

/*-------------------------------------------------------------------------------------*/
#define BUILTIN( cpp_t, rubytypes, cpp2ruby, ruby2cpp )                                 \
template<> struct ruby_value<cpp_t> {                                                   \
	static VALUE to  ( cpp_t value ) { return cpp2ruby; }                               \
	static cpp_t from( VALUE value ) { return ruby2cpp; }                               \
	static bool is_a( VALUE value ) {                                                   \
		static const VALUE klasses[] = { BOOST_PP_SEQ_ENUM(rubytypes) };                \
		const VALUE klass = CLASS_OF(value);                                            \
		for ( const VALUE* i = boost::begin(klasses); i != boost::end(klasses); ++i ) { \
			if (klass == *i) return true;                                               \
		}                                                                               \
		return false;                                                                   \
	}                                                                                   \
};                                                                                      \
template<> struct ruby_value<cpp_t const&> : ruby_value<cpp_t> {                        \
	static VALUE to  ( cpp_t const& value ) { return cpp2ruby; }                        \
	static cpp_t from( VALUE        value ) { return ruby2cpp; }                        \
};                                                                                      \
template<> struct ruby_value<cpp_t const>:ruby_value<cpp_t> {};                         \
template<> struct ruby_value<cpp_t&>:ruby_value<cpp_t const&>{};                        \
template<> struct class_registry<cpp_t> {                                               \
	static VALUE get() { assert(!"Should never happen!!!"); return Qnil; }              \
};                                                                                      \
template<> struct class_registry<cpp_t const >:class_registry<cpp_t> {};                \
template<> struct class_registry<cpp_t const&>:class_registry<cpp_t> {};                \
template<> struct class_registry<cpp_t&      >:class_registry<cpp_t> {};                \
/*-------------------------------------------------------------------------------------*/

	//// OWNERSHIP:  Gives a copy to the other language ///////////////////////////////////////////////////////////
#define BOOLTYPES (rb_cTrueClass)(rb_cFalseClass)
#define INTTYPES  (rb_cFixnum)(rb_cBignum)
	BUILTIN( bool          , BOOLTYPES    , value?Qtrue:Qfalse  , RTEST(value)                                );
	BUILTIN( signed   char , INTTYPES     , CHR2FIX(value)      , NUM2CHR(value)                              );
	BUILTIN( signed   short, INTTYPES     , INT2NUM(value)      , static_cast<short>(NUM2INT(value))          );
	BUILTIN( signed   int  , INTTYPES     , INT2NUM(value)      , NUM2INT(value)                              );
	BUILTIN( signed   long , INTTYPES     , LONG2NUM(value)     , NUM2LONG(value)                             );
	BUILTIN( unsigned char , INTTYPES     , UINT2NUM(value)     , static_cast<unsigned char >(NUM2CHR(value)) );
	BUILTIN( unsigned short, INTTYPES     , UINT2NUM(value)     , static_cast<unsigned short>(NUM2INT(value)) );
	BUILTIN( unsigned int  , INTTYPES     , UINT2NUM(value)     , NUM2INT(value)                              );
	BUILTIN( unsigned long , INTTYPES     , ULONG2NUM(value)    , NUM2LONG(value)                             );
	BUILTIN( float         , (rb_cFloat)  , rb_float_new(value) , static_cast<float>(NUM2DBL(value))          );
	BUILTIN( double        , (rb_cFloat)  , rb_float_new(value) , NUM2DBL(value)                              );
	BUILTIN( char*         , (rb_cString) , rb_str_new2(value)  , STR2CSTR(value)                             );
	BUILTIN( const char*   , (rb_cString) , rb_str_new2(value)  , STR2CSTR(value)                             );
	BUILTIN( std::string   , (rb_cString) , rb_str_new(value.empty()?"":value.c_str(),value.size()), STR2CSTR(value) );
#undef INTTYPES
#undef BOOLTYPES
#undef BUILTIN

}}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS
