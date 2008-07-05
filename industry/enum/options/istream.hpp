// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$
//
// TODO:  Expand options, refactor common code together ala ostream options

#ifndef IG_INDUSTRY_ENUM_OPTIONS_ISTREAM
#define IG_INDUSTRY_ENUM_OPTIONS_ISTREAM

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/range.hpp>
#include <algorithm>
#include <map>
#include <istream>
#include <string>
#include <cctype>

/////////////////////////////////// #define INDUSTRY_IMPL_ENUM_OPTION_is_* ( T , VALUES ) ///////
#define INDUSTRY_IMPL_ENUM_IS_VALUE_ARRAY( z, T, element ) std::map< std::string , T :: _ >::value_type( BOOST_PP_STRINGIZE(element) , T :: BOOST_PP_CAT(_,element) ),
/*------------------------------------------------------------------------------------*/
#define INDUSTRY_IMPL_ENUM_OPTION_is_any_case_naked( T, VALUES )                       \
inline std::istream& operator>>( std::istream& is, T & self ) {                        \
    std::string value;                                                                 \
    if (!(is >> value)) return is;                                                     \
    std::transform(value.begin(),value.end(),value.begin(),std::tolower);              \
    static const std::map< std::string, T::_ >::value_type                             \
        elements[] = { BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_IS_VALUE_ARRAY, T, VALUES ) }; \
    static const std::map< std::string, T::_ > mappings                                \
        ( boost::begin(elements), boost::end(elements) );                              \
    std::map< std::string, T::_ >::const_iterator match                                \
        = mappings.find(value);                                                        \
    if ( match == mappings.end() ) is.setstate( std::ios::failbit );                   \
    else self.value = match->second;                                                   \
    return is;                                                                         \
}                                                                                      \
/*-- Defaults/aliases: ---------------------------------------------------------------*/
#define INDUSTRY_IMPL_ENUM_OPTION_is_any_case INDUSTRY_IMPL_ENUM_OPTION_is_any_case_naked
#define INDUSTRY_IMPL_ENUM_OPTION_is          INDUSTRY_IMPL_ENUM_OPTION_is_any_case
////////////////////////////////////////////////////////////////////////////////////////

#endif //ndef IG_INDUSTRY_ENUM_OPTIONS_ISTREAM
