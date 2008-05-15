// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Outstanding Issues:
// Feb 01, 2007 - Added INDUSTRY_EXPORT #define -- did Washu botch a commit?  (Ruby stuff fails compile without)

#ifndef IG_INDUSTRY_CONFIG
#define IG_INDUSTRY_CONFIG

#ifndef INDUSTRY_TEMPLATE_ARGUMENT_LIMIT
#define INDUSTRY_TEMPLATE_ARGUMENT_LIMIT 10
//effect: allows:
//  industry::variable_template_arguments_class< A , B , T1 , T2 ... Tn >
//  where n <= INDUSTRY_TEMPLATE_ARGUMENT_LIMIT
#endif //ndef INDUSTRY_TEMPLATE_ARGUMENT_LIMIT

#ifndef INDUSTRY_METHOD_ARGUMENTS_LIMIT
#define INDUSTRY_METHOD_ARGUMENTS_LIMIT INDUSTRY_TEMPLATE_ARGUMENT_LIMIT
//effect: allows:
//  industry::method_interpreting_class< R ( T1 , T2 ... Tn ) >
//  where n <= INDUSTRY_METHOD_ARGUMENTS_LIMIT
#endif //ndef INDUSTRY_METHOD_ARGUMENTS_LIMIT


//INDUSTRY_OS_*       - OS definition macros
//INDUSTRY_OS_WINDOWS - Windows 95 or later
//INDUSTRY_OS_X       - OS X
//INDUSTRY_OS_LINUX   - Linux
//INDUSTRY_OS_UNKNOWN - Unknown OS

#if defined( _MSC_VER )
#define INDUSTRY_OS_WINDOWS

#elif defined( __APPLE__ )
#define INDUSTRY_OS_X

#elif defined( _LINUX )
#define INDUSTRY_OS_LINUX

#else
#define INDUSTRY_OS_UNKNOWN

#endif

//INDUSTRY_TYPEOF - The typeof keyword extension (if supported by the current compiler)
#if defined( __GNUC__ )
#define INDUSTRY_TYPEOF(x) __typeof__(x)

#endif

//INDUSTRY_EXPORT - ???
#ifndef INDUSTRY_EXPORT
#if defined( _USRDLL )
#define INDUSTRY_EXPORT __declspec(dllexport)
#else
#define INDUSTRY_EXPORT
#endif
#endif //ndef INDUSTRY_EXPORT

#endif //ndef IG_INDUSTRY_CONFIG
