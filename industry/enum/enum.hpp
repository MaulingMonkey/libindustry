// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ENUM_ENUM
#define IG_INDUSTRY_ENUM_ENUM

#include <industry/enum/basic_enum.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/expand.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

/////////////////// #define ENUM( name, options, values ) ////////////////////////////
#define INDUSTRY_IMPL_ENUM_CALL_OPTION( z, NAME_VALUES, OPTION )                     \
    BOOST_PP_CAT( INDUSTRY_IMPL_ENUM_OPTION_ , OPTION ) BOOST_PP_LPAREN()            \
    BOOST_PP_EXPAND( BOOST_PP_SEQ_ENUM(NAME_VALUES) )                                \
    BOOST_PP_RPAREN()                                                                \
/*----------------------------------------------------------------------------------*/
#define INDUSTRY_ENUM( name, options, values )                                       \
    INDUSTRY_BASIC_ENUM( name, values );                                             \
    BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_CALL_OPTION, (name)(values), options ) \
//////////////////////////////////////////////////////////////////////////////////////

#endif //ndef IG_INDUSTRY_ENUM_ENUM
