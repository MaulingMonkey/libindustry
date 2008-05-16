// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifdef RUBY_T_REMAP
// #define-d by registries.cpp
RUBY_T_REMAP( rb_cFalseClass, rb_cTrueClass );
RUBY_T_REMAP( rb_cFixnum    , rb_cBignum    );
#else // ndef RUBY_T_REMAP


#ifndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS
#define IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <map>
#include <string>

namespace industry { namespace languages { namespace ruby { namespace detail {
	template < typename T > struct class_registry;
	template < typename T > struct ruby_value;

/*----------------------------------------------------------------------*/
#define BUILTIN( cpp_t, ruby_t, cpp2ruby, ruby2cpp )                     \
template<> struct ruby_value<cpp_t> {                                    \
	static VALUE to  ( cpp_t value ) { return cpp2ruby; }                \
	static cpp_t from( VALUE value ) { return ruby2cpp; }                \
};                                                                       \
template<> struct ruby_value<cpp_t const&> {                             \
	static VALUE to  ( cpp_t const& value ) { return cpp2ruby; }         \
	static cpp_t from( VALUE        value ) { return ruby2cpp; }         \
};                                                                       \
template<> struct ruby_value<cpp_t const>:ruby_value<cpp_t> {};          \
template<> struct ruby_value<cpp_t&>:ruby_value<cpp_t const&>{};         \
template<> struct class_registry<cpp_t> {                                \
	static VALUE get() { return ruby_t; }                                \
};                                                                       \
template<> struct class_registry<cpp_t const >:class_registry<cpp_t> {}; \
template<> struct class_registry<cpp_t const&>:class_registry<cpp_t> {}; \
template<> struct class_registry<cpp_t&      >:class_registry<cpp_t> {}; \
/*----------------------------------------------------------------------*/

	//// OWNERSHIP:  Gives a copy to the other language ///////////////////////////////////////////////////////////
	BUILTIN( bool          , rb_cTrueClass, value?Qtrue:Qfalse  , RTEST(value)                                );
	BUILTIN( signed   char , rb_cBignum   , CHR2FIX(value)      , NUM2CHR(value)                              );
	BUILTIN( signed   short, rb_cBignum   , INT2NUM(value)      , static_cast<short>(NUM2INT(value))          );
	BUILTIN( signed   int  , rb_cBignum   , INT2NUM(value)      , NUM2INT(value)                              );
	BUILTIN( signed   long , rb_cBignum   , LONG2NUM(value)     , NUM2LONG(value)                             );
	BUILTIN( unsigned char , rb_cBignum   , UINT2NUM(value)     , static_cast<unsigned char >(NUM2CHR(value)) );
	BUILTIN( unsigned short, rb_cBignum   , UINT2NUM(value)     , static_cast<unsigned short>(NUM2INT(value)) );
	BUILTIN( unsigned int  , rb_cBignum   , UINT2NUM(value)     , NUM2INT(value)                              );
	BUILTIN( unsigned long , rb_cBignum   , ULONG2NUM(value)    , NUM2LONG(value)                             );
	BUILTIN( float         , rb_cFloat    , rb_float_new(value) , static_cast<float>(NUM2DBL(value))          );
	BUILTIN( double        , rb_cFloat    , rb_float_new(value) , NUM2DBL(value)                              );
	BUILTIN( char*         , rb_cString   , rb_str_new2(value)  , STR2CSTR(value)                             );
	BUILTIN( const char*   , rb_cString   , rb_str_new2(value)  , STR2CSTR(value)                             );
	BUILTIN( std::string   , rb_cString   , rb_str_new(value.empty()?"":value.c_str(),value.size()), STR2CSTR(value) );

#undef BUILTIN

}}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_BUILTINS
#endif //ndef RUBY_T_MAP
