// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - industry.type.hpp => industry/type.hpp
// May 27, 2006 - Bugfix (missing return keyword :S) & stupid warning supression
// May 21, 2006 - Created

#ifndef IG_INDUSTRY_TYPE
#define IG_INDUSTRY_TYPE

#include <functional>
#include <typeinfo>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4800 ) //forcing int => bool performance warning
#endif //def _MSC_VER

namespace industry {
	class type {
		unsigned info;
	public:
		type( void ) throw(): info( 0 ) {}
		type( const std::type_info& src ); // can throw std::bad_alloc
		type( const type & other ) throw() : info( other.info ) {}

		const char* name() const throw();
		unsigned key() const { return info; }
		
		friend bool operator==( const type & lhs , const type & rhs ) { return lhs.info == rhs.info; }
		friend bool operator< ( const type & lhs , const type & rhs ) { return lhs.info  < rhs.info; }

		template < typename T > static type id() { static const type t(typeid(T)); return t; }
	};

	template < typename T > inline type dynamic_type_of( const T& expr ) { return typeid(expr); }
	template < typename T > inline type static_type_of ( const T& expr ) { return type::id<T>(); }
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif //def _MSC_VER

#endif //ndef IG_INDUSTRY_TYPE
