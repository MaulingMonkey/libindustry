// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ENUM_OPTIONS_OSTREAM
#define IG_INDUSTRY_ENUM_OPTIONS_OSTREAM

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <ostream>
#include <cctype>

//////////////////////////////////////////// #define IMPL_ENUM_OPTION_os_* ( T , VALUES ) \  ///////////////////////
#define INDUSTRY_IMPL_ENUM_PRE_OS_OPTION(T)  inline std::ostream& operator<<( std::ostream& os, const T & self ) { \
                                                 switch ( self.value ) {                                        // |
//                                                   BOOST_PP_SEQ_FOR_EACH( IMPL_ENUM_OS_SWITCH_CASE_os_*, T, VALUES )
#define INDUSTRY_IMPL_ENUM_POST_OS_OPTION()      default:                                                          \
                                                     assert(!"Invalid value state");                               \
                                                 }                                                                 \
                                                 return os;                                                        \
                                             }                                                                     \
/*-- Unique implementations: -------------------------------------------------------------------------------------*/
#define INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_same_case_naked(  z, T, element ) case T :: element: { static const char s[] = BOOST_PP_STRINGIZE(element); /* no changes */                                os << s; } break;
#define INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_capitalize_naked( z, T, element ) case T :: element: { static       char s[] = BOOST_PP_STRINGIZE(element); s[0] = std::upcase(s[0]);                       os << s; } break;
#define INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_upper_case_naked( z, T, element ) case T :: element: { static       char s[] = BOOST_PP_STRINGIZE(element); std::transform(s,boost::end(s),s,std::upcase);  os << s; } break;
#define INDUSTRY_IMPL_ENUM_OPTION_os_same_case_naked(  T, VALUES ) INDUSTRY_IMPL_ENUM_PRE_OS_OPTION(T) BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_same_case_naked , T, VALUES ) INDUSTRY_IMPL_ENUM_POST_OS_OPTION()
#define INDUSTRY_IMPL_ENUM_OPTION_os_capitalize_naked( T, VALUES ) INDUSTRY_IMPL_ENUM_PRE_OS_OPTION(T) BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_capitalize_naked, T, VALUES ) INDUSTRY_IMPL_ENUM_POST_OS_OPTION()
#define INDUSTRY_IMPL_ENUM_OPTION_os_upper_case_naked( T, VALUES ) INDUSTRY_IMPL_ENUM_PRE_OS_OPTION(T) BOOST_PP_SEQ_FOR_EACH( INDUSTRY_IMPL_ENUM_OS_SWITCH_CASE_os_upper_case_naked, T, VALUES ) INDUSTRY_IMPL_ENUM_POST_OS_OPTION()
/*-- Defaults/aliases: -------------------------------------------------------------------------------------------*/
#define INDUSTRY_IMPL_ENUM_OPTION_os_same_case  INDUSTRY_IMPL_ENUM_OPTION_os_same_case_naked
#define INDUSTRY_IMPL_ENUM_OPTION_os            INDUSTRY_IMPL_ENUM_OPTION_os_same_case
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //ndef IG_INDUSTRY_ENUM_OPTIONS_OSTREAM
