// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ENUM_BASIC_ENUM
#define IG_INDUSTRY_ENUM_BASIC_ENUM

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>

#define INDUSTRY_IMPL_ENUM_RAW_FILTER( r, unused_data, e ) BOOST_PP_CAT(_,e),
#define INDUSTRY_IMPL_ENUM_F( r, unused_data, e ) static void e( enumeration<BOOST_PP_CAT(_,e)> ) {}

////////////////////////////////////////////////////////////////////
#define INDUSTRY_BASIC_ENUM( T, values )                           \
class T {                                                          \
public:                                                            \
	enum _ { BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_RAW_FILTER, ~, values ) count }; \
	_ value;                                                       \
private:                                                           \
	template < _ > class enumeration {};                           \
public:                                                            \
	BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_F, ~, values )       \
                                                                   \
    T(): value( BOOST_PP_CAT(_,BOOST_PP_SEQ_HEAD(values)) ) {}     \
	template < _ value > T( void (*)( enumeration<value> ) ): value(value) {} \
    T( _ value ): value(value) {}                                  \
};                                                                 \
bool operator==( T lhs, T rhs ) { return lhs.value == rhs.value; } \
bool operator!=( T lhs, T rhs ) { return lhs.value != rhs.value; } \
bool operator==( T lhs, T::_ rhs ) { return lhs.value == rhs; }    \
bool operator!=( T lhs, T::_ rhs ) { return lhs.value != rhs; }    \
bool operator==( T::_ lhs, T rhs ) { return lhs == rhs.value; }    \
bool operator!=( T::_ lhs, T rhs ) { return lhs != rhs.value; }    \
////////////////////////////////////////////////////////////////////

#endif //ndef IG_INDUSTRY_ENUM_BASIC_ENUM
