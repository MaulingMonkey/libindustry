// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/languages/ruby/value.hpp>

namespace industry { namespace languages { namespace ruby {
	value::value()
		: value_( Qnil )
	{
		rb_gc_register_address(&value_);
	}
	value::value( const value& v )
		: value_(v.value_)
	{
		rb_gc_register_address(&value_);
	}
	value::value( const lazy_value& v )
		: value_(v.value_)
	{
		rb_gc_register_address(&value_);
	}
	value::value( VALUE v )
		: value_(v)
	{
		rb_gc_register_address(&value_);
	}
	value::value( double d )
		: value_(DOUBLE2NUM(d))
	{
		rb_gc_register_address(&value_);
	}
	value::value( int i )
		: value_(INT2NUM(i))
	{
		rb_gc_register_address(&value_);
	}
	value::value( unsigned int i )
		: value_(UINT2NUM(i))
	{
		rb_gc_register_address(&value_);
	}
	value::value( const char * str )
		: value_(rb_str_new2(str))
	{
		rb_gc_register_address(&value_);
	}
	value::value( const std::string& str )
		: value_(rb_str_new(str.c_str(),str.size()))
	{
		rb_gc_register_address(&value_);
	}
	value::~value()
	{
		rb_gc_unregister_address(&value_);
	}
}}}
