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
#include <boost/preprocessor/seq/seq.hpp>

////////////////////////////////////////////////////////////////////
#define INDUSTRY_BASIC_ENUM( T, values )                           \
class T {                                                          \
public:                                                            \
    enum _ { BOOST_PP_SEQ_ENUM(values) };                          \
    _ value;                                                       \
                                                                   \
    T(): value( BOOST_PP_SEQ_HEAD(values) ) {}                     \
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
