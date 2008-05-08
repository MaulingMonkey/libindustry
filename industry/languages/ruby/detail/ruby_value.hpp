// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE

#include <boost/intrusive_ptr.hpp>
#include <boost/ref.hpp>
#include <memory>
#include <string>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 ) // conditional expression is constant -- do {...} while(0) idiom used by Data_Wrap_Struct
#endif

namespace industry { namespace languages { namespace ruby {
	template<class T> struct class_;
	namespace detail {
		template<class T> struct ruby_value;

		template<class T> struct ruby_value<T*> { // Ownership does not transfer
			static VALUE to(T* ptr) { return Data_Wrap_Struct(class_<T>::get_class(), 0, 0, ptr); }
			static T* from(VALUE v) { T* ptr; Data_Get_Struct(v, T, ptr); return ptr; }
		};
		template<class T> struct ruby_value<const T*> { // Ownership does not transfer
			static VALUE to(const T* ptr) { return Data_Wrap_Struct(class_<T>::get_class(), 0, 0, ptr); }
			static const T* from(VALUE v) { const T* ptr; Data_Get_Struct(v, const T, ptr); return ptr; }
		};

		// TODO: Prevent [c]refs to builtin types?
		template<class T> struct ruby_value< boost::reference_wrapper<T> > { // Ownership does not transfer
			static VALUE to( const boost::reference_wrapper<T>& ref ) { return Data_Wrap_Struct(class_<T>::get_class(), 0, 0, ref.get_pointer()); }
			// no from -- use ruby_value<T&>::from instead
		};
		template<class T> struct ruby_value< boost::reference_wrapper<const T> > { // Ownership does not transfer
			/*
			 * We're not so masochistic as to try and get Ruby to respect constness, are we?
			 * Maybe const_cast a freezed VALUE?  Probably leave this to be done explicitly by the end user.
			 */
			// no from -- use ruby_value<const T&>::from instead
		};

		template<class T> struct ruby_value< std::auto_ptr<T> > { // Ownership DOES transfer
			static VALUE to(std::auto_ptr<T> ptr) { return Data_Wrap_Struct(class_<T>::get_class(), 0, class_<T>::free_type, ptr.release()); }
		};
		template<class T> struct ruby_value< std::auto_ptr<const T> > { // Ownership DOES transfer
			static VALUE to(std::auto_ptr<const T> ptr) { return Data_Wrap_Struct(class_<T>::get_class(), 0, class_<T>::free_type, ptr.release()); }
		};

		// TODO: class_ probably needs to handle init differently for intrusive_ptr<T> enabled classes
		template<class T> struct ruby_value< boost::intrusive_ptr<T> > {
			static VALUE to(const boost::intrusive_ptr<T>& ptr ) { add_ref(ptr.get()); return Data_Wrap_Struct(class_<T>::get_class(), 0, release, ptr.get()); }
			static boost::intrusive_ptr<T> from(VALUE v) { T* ptr; Data_Get_Struct(v,T,ptr); return ptr; }
		private:
			// TODO: Work around ADL-missing compilers by playing in namespace boost::* ?
			static void add_ref( T* ptr ) { intrusive_ptr_add_ref(ptr); }
			static void release( T* ptr ) { intrusive_ptr_release(ptr); }
		};

		template<class T> struct ruby_value< boost::intrusive_ptr<const T> > {
			static VALUE to(const boost::intrusive_ptr<const T>& ptr ) { add_ref(ptr.get()); return Data_Wrap_Struct(class_<T>::get_class(), 0, release, ptr.get()); }
			static boost::intrusive_ptr<const T> from(VALUE v) { const T* ptr; Data_Get_Struct(v,T,ptr); return ptr; }
		private:
			// TODO: Work around ADL-missing compilers by playing in namespace boost::* ?
			static void add_ref( const T* ptr ) { intrusive_ptr_add_ref(ptr); }
			static void release( const T* ptr ) { intrusive_ptr_release(ptr); }
		};

		// No boost::scoped_ptr<T> -- use get() to explicitly use non-transfering ownership
		// No boost::shared_ptr<T> -- use get() to explicitly use non-transfering ownership
		//    TODO:  Write ownership sharing to/from for shared_ptr, at least when using enable_shared_from_this?

		template <class T> struct ruby_value< const T > : ruby_value<T> {};

		// FIXME:  ruby_value< const T& >::from overrides ruby_value<char, short, std::string, etc>
		template <class T> struct ruby_value< const T&> : ruby_value<const T> {
			// Uses ruby_value<const T>::to -- use boost::cref to pass to ruby by reference!
			static const T& from( VALUE v ) { const T* ptr; Data_Get_Struct(v,const T,ptr); return *ptr; }
		};
		template <class T> struct ruby_value<       T&> : ruby_value<T> {
			// Uses ruby_value<T>::to -- use boost::ref to pass to ruby by reference!
			static T& from( VALUE v ) { T* ptr; Data_Get_Struct(v,T,ptr); return *ptr; }
		};

		template<class T> struct ruby_value {
			static VALUE to(const T& ref) { return class_<T>::clone_type(ref); }
			static T from(VALUE v) { T* ptr; Data_Get_Struct(v, T, ptr); return *ptr; }
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
		template<> struct ruby_value<      std::string > { static VALUE to(std::string const& v) { return rb_str_new(v.c_str(), v.length()); } static std::string from(VALUE v) {return STR2CSTR(v); } };
		template<> struct ruby_value<const std::string&> { static VALUE to(std::string const& v) { return rb_str_new(v.c_str(), v.length()); } static std::string from(VALUE v) {return STR2CSTR(v); } };
	}
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
