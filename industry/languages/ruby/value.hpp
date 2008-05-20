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
#include <industry/languages/ruby/exceptions.hpp>
#include <boost/preprocessor.hpp>
#include <boost/type_traits.hpp>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iterator>

namespace industry { namespace languages { namespace ruby {
	class value;
	class lazy_value {
		friend class value;
		friend struct detail::ruby_value<lazy_value>;
		VALUE value_;

		lazy_value();
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

		// TODO:  operator=(value)
	};

	class value {
		friend struct detail::ruby_value<value>;
		VALUE value_;
	public:
		value();
		value( const value     & );
		value( const lazy_value& );
		value( VALUE );

		value( double );
		value( int    );
		value( unsigned int );
		value( const char * );
		value( const std::string& );

		template < typename Iterator >
		value( Iterator begin, Iterator end ) {
			value_ = Qnil;
			rb_gc_register_address(&value_);
			value_ = rb_ary_new();
			
			for ( Iterator i = begin ; i != end ; ++i ) {
				VALUE element = detail::ruby_value< typename std::iterator_traits<Iterator>::value_type >::to(*i);
				rb_ary_push( value_, element );
			}
		}

		~value();


		value_function operator->*( const char *       str ) const { return value_function(value_,str); }
		value_function operator->*( const std::string& str ) const { return value_function(value_,str.c_str(),str.size()); }
		template < size_t N >
		value_function operator->*( const char (&str)[N]   ) const { return value_function(value_,str,N-1); }

		template < typename T > T to() const { return detail::ruby_value<T>::from(value_); }

	private:
		typedef void (value::*bool_type)() const;
		void true_bool() const {}
	public:
		operator bool_type() const { return RTEST(value_) ? &value::true_bool : 0; }
		VALUE get_value() const { return value_; }

		friend value operator! ( const value& self ) { return self ? Qfalse : Qtrue ; }
		friend value operator+ ( const value& self ) { return (self ->* "+@")(); }
		friend value operator- ( const value& self ) { return (self ->* "-@")(); }
		friend value operator~ ( const value& self ) { return (self ->* "~")(); }

		friend value operator==( const value& lhs, const value& rhs ) { return (lhs ->* "==")(rhs); }
		friend value operator!=( const value& lhs, const value& rhs ) { return (lhs ->* "!=")(rhs); }
		friend value operator||( const value& lhs, const value& rhs ) { return lhs ? lhs : rhs; }
		friend value operator&&( const value& lhs, const value& rhs ) { return lhs ? rhs ? lhs : Qfalse : Qfalse; }
		friend value operator+ ( const value& lhs, const value& rhs ) { return (lhs ->* "+" )(rhs); }
		friend value operator- ( const value& lhs, const value& rhs ) { return (lhs ->* "-" )(rhs); }
		friend value operator* ( const value& lhs, const value& rhs ) { return (lhs ->* "*" )(rhs); }
		friend value operator/ ( const value& lhs, const value& rhs ) { return (lhs ->* "/" )(rhs); }
		friend value operator% ( const value& lhs, const value& rhs ) { return (lhs ->* "%" )(rhs); }
		friend value operator& ( const value& lhs, const value& rhs ) { return (lhs ->* "&" )(rhs); }
		friend value operator| ( const value& lhs, const value& rhs ) { return (lhs ->* "|" )(rhs); }
		friend value operator^ ( const value& lhs, const value& rhs ) { return (lhs ->* "^" )(rhs); }
		friend value operator<<( const value& lhs, const value& rhs ) { return (lhs ->* "<<")(rhs); }
		friend value operator>>( const value& lhs, const value& rhs ) { return (lhs ->* ">>")(rhs); }
		friend value operator<=( const value& lhs, const value& rhs ) { return (lhs ->* "<=")(rhs); }
		friend value operator>=( const value& lhs, const value& rhs ) { return (lhs ->* ">=")(rhs); }
		friend value operator< ( const value& lhs, const value& rhs ) { return (lhs ->* "<" )(rhs); }
		friend value operator> ( const value& lhs, const value& rhs ) { return (lhs ->* ">" )(rhs); }

		friend std::ostream& operator<<( std::ostream& os, const value& v ) {
			os << value(rb_inspect(v.value_)).to<const char*>();
			return os;
		}
		
		template<class Destination, class Source>
		friend Destination value_static_cast(const value& self) {
			typedef detail::class_registry<typename boost::remove_cv<typename boost::remove_reference<typename boost::remove_pointer<Source>::type >::type >::type > class_type;
			
			if(rb_class_inherited_p(class_type::get(), CLASS_OF(self.value_)) != Qnil) {
				return static_cast<Destination>(detail::ruby_value<Source>::from(self.value_));
			}
			if(boost::is_pointer<Destination>::value)
				return 0;
			throw std::bad_cast();
		}

		template<class Destination, class Source>
		friend Destination value_dynamic_cast(const value& self) {
			typedef detail::class_registry<typename boost::remove_cv<typename boost::remove_reference<typename boost::remove_pointer<Source>::type>::type>::type> class_type;

			if(rb_class_inherited_p(class_type::get(), CLASS_OF(self.value_)) != Qnil) {
				return dynamic_cast<Destination>(detail::ruby_value<Source>::from(self.value_));
			}
			if(boost::is_pointer<Destination>::value)
				return 0;
			throw std::bad_cast();
		}

		template<class T>
		value operator[] (T const& index) const { return ((*this) ->* "[]")(index); }
		lazy_value operator[] (std::size_t index) const { return CLASS_OF(value_) == rb_cArray ? *(RARRAY(value_)->ptr + index) : CLASS_OF(value_) == rb_cHash ? rb_hash_lookup(value_, detail::ruby_value<std::size_t>::to(index)) : ((*this) ->* "[]")(index); }
		
		std::size_t length() const { return ((*this) ->* "length")().to<std::size_t>(); }

		value get_type() { return rb_class_of(value_); }
		// TODO: operator[]
		// TODO: assignment operators
	};

	namespace detail {
		template <> struct ruby_value<lazy_value> { static VALUE to( const lazy_value& v ) { return v.value_; } /* no corresponding from() */ };
		template <> struct ruby_value<value     > { static VALUE to( const value     & v ) { return v.value_; } static value from( VALUE v ) { return value(v); } };
	}
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_VALUE
