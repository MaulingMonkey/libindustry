// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE

#include <string>
#include <iostream>

namespace industry { namespace languages { namespace ruby { namespace detail {
	template<class T> struct class_;
	template<class T> struct ruby_value;

	template<class T> struct ruby_value<T*> {
		static VALUE to(T* ptr) {
			return Data_Wrap_Struct(class_<T>::get_class(), 0, class_<T>::free_type, ptr);
		}

		static T* from(VALUE v) {
			T* ptr;
			Data_Get_Struct(v, T, ptr);
			return ptr;
		}
	};

	template<class T> struct ruby_value<const T*> {
		static VALUE to(const T* ptr) {
			return Data_Wrap_Struct(class_<T>::get_class(), 0, class_<T>::free_type, ptr);
		}

		static const T* from(VALUE v) {
			const T* ptr;
			Data_Get_Struct(v, const T, ptr);
			return ptr;
		}
	};

	template<> struct ruby_value<char>  { static VALUE to(char  v) { return CHR2FIX(v); } static char  from(VALUE v) {return NUM2CHR(v);  } };
	template<> struct ruby_value<short> { static VALUE to(short v) { return INT2NUM(v); } static short from(VALUE v) {return static_cast<short>(NUM2INT(v)); } };
	template<> struct ruby_value<int>   { static VALUE to(int   v) { return INT2NUM(v); } static int   from(VALUE v) {return NUM2INT(v);  } };
	template<> struct ruby_value<long>  { static VALUE to(long  v) { return LONG2NUM(v);} static long  from(VALUE v) {return NUM2LONG(v); } };
	template<> struct ruby_value<unsigned char>  { static VALUE to(unsigned char  v) { return UINT2NUM(v); } static unsigned char  from(VALUE v) {return static_cast<unsigned char >(NUM2UINT(v)); } };
	template<> struct ruby_value<unsigned short> { static VALUE to(unsigned short v) { return UINT2NUM(v); } static unsigned short from(VALUE v) {return static_cast<unsigned short>(NUM2UINT(v)); } };
	template<> struct ruby_value<unsigned int>   { static VALUE to(unsigned int   v) { return UINT2NUM(v); } static unsigned int   from(VALUE v) {return NUM2UINT(v); } };
	template<> struct ruby_value<unsigned long>  { static VALUE to(unsigned long  v) { return ULONG2NUM(v);} static unsigned long  from(VALUE v) {return NUM2ULONG(v); } };

	template<> struct ruby_value<float>  { static VALUE to(float  v) { return rb_float_new(v); } static float  from(VALUE v) { return (float)NUM2DBL(v); } };
	template<> struct ruby_value<double> { static VALUE to(double v) { return rb_float_new(v); } static double from(VALUE v) { return        NUM2DBL(v); } };

	template<> struct ruby_value<char*> { static VALUE to(char* v) { return rb_str_new2(v); } static char* from(VALUE v) {return STR2CSTR(v); } };
	template<> struct ruby_value<const char*> { static VALUE to(const char* v) { return rb_str_new2(v); } static const char* from(VALUE v) {return STR2CSTR(v); } };
	template<> struct ruby_value<std::string> { static VALUE to(std::string const& v) { return rb_str_new(v.c_str(), v.length()); } static std::string from(VALUE v) {return STR2CSTR(v); } };
}}}}

#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
