// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_ARRAY_ITERATOR
#define IG_INDUSTRY_LANGUAGES_RUBY_ARRAY_ITERATOR

#include <industry/languages/ruby/exceptions.hpp>
#include <industry/languages/ruby/value.hpp>
#include <boost/range.hpp>

namespace industry { namespace languages { namespace ruby {
	class value;

	template < typename T = value >
	class array_iterator {
		value array;
		VALUE* position;
	public:
		typedef std::random_access_iterator_tag         iterator_category;
		typedef std::ptrdiff_t                          difference_type;
		typedef T                                       value_type;
		typedef T                                       reference;
		typedef void                                    pointer;

		array_iterator(): array(Qnil), position(0) {}

		static array_iterator begin( value array ) {
			if ( CLASS_OF(array.get_value()) != rb_cArray ) throw ruby_type_error( "Expected an Array, got a %1%", rb_cArray );

			array_iterator i;
			i.array = array;
			i.position = RARRAY(array.get_value())->ptr;

			return i;
		}
		static array_iterator end  ( value array ) {
			if ( CLASS_OF(array.get_value()) != rb_cArray ) throw ruby_type_error( "Expected an Array, got a %1%", rb_cArray );

			array_iterator i;
			i.array = array;
			i.position = RARRAY(array.get_value())->ptr + RARRAY(array.get_value())->len;

			return i;
		}

		reference         operator* () const { return detail::ruby_value<reference>::from(*position); }
		// pointer           operator->() const { ... }
		array_iterator&   operator++(   ) { assert(array.get_value() && position); ++position; return *this; }
		array_iterator    operator++(int) { array_iterator copy(*this); ++*this; return copy; }
		array_iterator&   operator--(   ) { assert(array.get_value() && position); --position; return *this; }
		array_iterator    operator--(int) { array_iterator copy(*this); --*this; return copy; }
		array_iterator&   operator+=(ptrdiff_t d) { assert(array.get_value() && position); position+=d; return *this; }
		array_iterator    operator+ (ptrdiff_t d) { array_iterator copy(*this); copy+=d; return copy; }
		array_iterator&   operator-=(ptrdiff_t d) { return *this += -d; }
		array_iterator    operator- (ptrdiff_t d) { return *this +  -d; }
		reference         operator[](ptrdiff_t d) { return *(*this+d); }
		friend  ptrdiff_t operator- ( const array_iterator& lhs , const array_iterator& rhs ) { assert( lhs.array.get_value() == rhs.array.get_value() ); return lhs.position - rhs.position; }
		friend  bool      operator< ( const array_iterator& lhs , const array_iterator& rhs ) { assert( lhs.array.get_value() == rhs.array.get_value() ); return lhs - rhs < 0; }
		friend  bool      operator> ( const array_iterator& lhs , const array_iterator& rhs ) { assert( lhs.array.get_value() == rhs.array.get_value() ); return lhs - rhs > 0; }
		friend  bool      operator==( const array_iterator& lhs , const array_iterator& rhs ) { assert( lhs.array.get_value() == rhs.array.get_value() ); return lhs.position == rhs.position; }
		friend  bool      operator!=( const array_iterator& lhs , const array_iterator& rhs ) { assert( lhs.array.get_value() == rhs.array.get_value() ); return lhs.position != rhs.position; }
	};

	template < typename T > boost::iterator_range< array_iterator<T> > as_array_of( const value& array ) {
		return boost::iterator_range< array_iterator<T> >( array_iterator<T>::begin(array), array_iterator<T>::end(array) );
	}
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_ARRAY_ITERATOR
