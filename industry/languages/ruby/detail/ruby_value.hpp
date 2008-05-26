// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
//     Notes:
// Ownership of intrusive_ptr<T> enabled classes is always shared despite descriptions bellow, except where copied.
// You will still need to use intrusive_ptr<T> or equivalent, of course, on the C++ side of things.
//
//     Missing:
// ruby_value doesn't handle shared_ptr<T> or scoped_ptr<T> and probably never will.  Sharing their ownership
// is a PITA or impossible, and .get() should be used to make explicit the lack of ownership transfer.
// Types lacking ruby_value<...>::from are unidirectional from C++ to Ruby
// ruby_value<...>::to is usually missing due to constness and Ruby not respecting it (yet).
//
//     See also:
// industry/languages/ruby/detail/builtins.hpp for the specializations of "value" (always copied) types

#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE

#include <industry/languages/ruby/declarations.hpp>
#include <industry/languages/ruby/detail/builtins.hpp>
#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/preprocessor.hpp>
#include <boost/ref.hpp>
#include <boost/tuple/tuple.hpp>
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 ) // conditional expression is constant -- the do {...} while(0) idiom used by Data_Wrap_Struct
#endif

namespace industry { namespace languages { namespace ruby {
	namespace detail {
		template<class T> struct class_registry;
		template<class T> struct instance_registry;
		template<class T> struct ruby_value;



		//// OWNERSHIP:  Does not transfer ////////////////////////////////////////////////////////////////////////////
		template<class T> struct ruby_value<T*> {
			static VALUE to(T* ptr) { return ptr ? instance_registry<T>::get_ruby_value(ptr) : Qnil; }
			static T* from(VALUE v) { if (v==Qnil) return 0; T* ptr; Data_Get_Struct(v, T, ptr); return ptr; }
		};
		template<class T> struct ruby_value<const T*> {
			static const T* from(VALUE v) { if (v==Qnil) return 0; const T* ptr; Data_Get_Struct(v, const T, ptr); return ptr; }
		};

		template<class T> struct ruby_value< boost::reference_wrapper<T> > {
			static VALUE to( const boost::reference_wrapper<T>& ref ) { return instance_registry<T>::get_ruby_value(ref.get_pointer()); }
		};
		
		//// OWNERSHIP:  Transfers (to Ruby) //////////////////////////////////////////////////////////////////////////
		template<class T> struct ruby_value< std::auto_ptr<T> > {
			static VALUE to(std::auto_ptr<T> ptr) { return ptr.get() ? instance_registry<T>::register_ruby_owned(ptr.release()) : Qnil; }
		};

		//// OWNERSHIP:  Shared (always) //////////////////////////////////////////////////////////////////////////////
		// (must implement the intrusive_ptr interface, of course)
		template<class T> struct ruby_value< boost::intrusive_ptr<T> > {
			static VALUE to(const boost::intrusive_ptr<T>& ptr ) { return ptr.get() ? instance_registry<T>::get_ruby_value(ptr.get()) : Qnil; }
			static boost::intrusive_ptr<T> from(VALUE v) { if (v==Qnil) return boost::intrusive_ptr<T>(); T* ptr; Data_Get_Struct(v,T,ptr); return ptr; }
		};

		template<class T> struct ruby_value< boost::intrusive_ptr<const T> > {
			static boost::intrusive_ptr<const T> from(VALUE v) { if (v==Qnil) return boost::intrusive_ptr<const T>(); const T* ptr; Data_Get_Struct(v,T,ptr); return ptr; }
		};

		//// OWNERSHIP:  Gives a copy to Ruby, but does not copy or transfer ownership to C++ /////////////////////////
		template <class T> struct ruby_value< const T > : ruby_value<T> {};
		template <class T> struct ruby_value< const T&> : ruby_value<const T> {
			// Uses ruby_value<const T>::to if available -- use boost::ref to pass to ruby by reference!
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

		//// OWNERSHIP:  Gives a copy to the other language ///////////////////////////////////////////////////////////
		// (however, the ownership of inner types is dealt with with their own policies)
		template< typename L, typename R > struct ruby_value< std::pair<L,R> > {
			static VALUE to( const std::pair<L,R>& p ) {
				return rb_ary_new3( 2, ruby_value<L>::to(p.first), ruby_value<R>::to(p.second) );
			}
			static std::pair<L,R> from( VALUE v ) {
				if ( CLASS_OF(v) != rb_cArray || RARRAY(v)->len != 2 ) {
					throw std::runtime_error( "Expected an array of length 2" );
				}
				return std::pair<L,R>( ruby_value<L>::from(rb_ary_entry(v,0)), ruby_value<R>::from(rb_ary_entry(v,1)) );
			}
		};
		template< typename L, typename R > struct ruby_value< const std::pair<L,R> > : ruby_value< std::pair<L,R> > {};

#define TO_RUBY_VALUE(z,n,tuple) ruby_value<T##n>::to(tuple.template get<n>())
#define FROM_RUBY_VALUE(z,n,unused) ruby_value<T##n>::from(rb_ary_entry(v,n))
/*---------------------------------------------------------------------------------------------------------------------*/
#define BOOST_PP_LOCAL_MACRO(N)                                                                                         \
		template < BOOST_PP_ENUM_PARAMS(N,typename T) > struct ruby_value< boost::tuple<BOOST_PP_ENUM_PARAMS(N,T)> > {  \
			typedef boost::tuple<BOOST_PP_ENUM_PARAMS(N,T)> tuple_type;                                                 \
			static VALUE to( const tuple_type& t ) { return rb_ary_new3(N,BOOST_PP_ENUM(N,TO_RUBY_VALUE,t)); }          \
			static tuple_type from( VALUE v ) {                                                                         \
				if ( CLASS_OF(v) != rb_cArray || RARRAY(v)->len != N ) {                                                \
					throw std::runtime_error( "Expected an array of length " #N );                                      \
				}                                                                                                       \
				return tuple_type(BOOST_PP_ENUM(N,FROM_RUBY_VALUE,~));                                                  \
			}                                                                                                           \
		};                                                                                                              \
		template < BOOST_PP_ENUM_PARAMS(N,typename T) >                                                                 \
		struct ruby_value< const boost::tuple<BOOST_PP_ENUM_PARAMS(N,T)>& >                                             \
			:  ruby_value< boost::tuple<BOOST_PP_ENUM_PARAMS(N,T)> > {};                                                \
/*---------------------------------------------------------------------------------------------------------------------*/
#define BOOST_PP_LOCAL_LIMITS (1,10)
#include BOOST_PP_LOCAL_ITERATE()
#undef TO_RUBY_VALUE
#undef FROM_RUBY_VALUE
#undef BOOST_PP_LOCAL_MACRO
	}
}}}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_RUBY_VALUE
