// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 25, 2006 - industry.config.hpp => industry/config.hpp
// Aug 17, 2006 - INDUSTRY_OS_APPLE => INDUSTRY_OS_X
// May 27, 2006 - Added explanitory examples
// May 20, 2006 - Created

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
#if defined( _MSC_VER )
#define INDUSTRY_OS_WINDOWS
#elif defined( __APPLE__ )
#define INDUSTRY_OS_X
#else
#error "Unknown platform!"
#endif

//INDUSTRY_UNICODE_DEFAULT - Defacto i/o mode (e.g. TCHAR type, streams to use, etc)
//INDUSTRY_UNICODE_ENABLED - Wheither or not unicode is available
#if defined( _MSC_VER )
	#define INDUSTRY_UNICODE_ENABLED
	#if defined( UNICODE )
		#define INDUSTRY_UNICODE_DEFAULT
	#endif
#endif

#endif //ndef IG_INDUSTRY_CONFIG