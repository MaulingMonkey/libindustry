// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ENUM_OPTIONS_SCOPE
#define IG_INDUSTRY_ENUM_OPTIONS_SCOPE

#include <boost/preprocessor/seq/for_each.hpp>

#define INDUSTRY_IMPL_ENUM_DUP_IN_OUTER_SCOPE( r, T, value ) const T value = T :: value;
/*-------------------------------------------------------------------------------*/
#define INDUSTRY_IMPL_ENUM_OPTION_duplicate_constants_in_outer_scope( T, VALUES ) \
namespace {                                                                       \
    BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_DUP_IN_OUTER_SCOPE, T, VALUES )     \
}                                                                                 \
/*-------------------------------------------------------------------------------*/

#endif //ndef IG_INDUSTRY_ENUM_OPTIONS_SCOPE
