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
	value::~value()
	{
		rb_gc_unregister_address(&value_);
	}
}}}
