// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Jul 21, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_IMPORT
#define IG_INDUSTRY_API_WINDOWS_IMPORT

#include <industry/config.hpp>

#if !defined( INDUSTRY_OS_WINDOWS )
#error "Cannot import the windows API when not building for windows"


#elif defined( _MSC_VER ) && !defined( _MSC_EXTENSIONS )
#error "Language extensions must be enabled for anything involving windows.h to compile"

#else //defined( INDUSTRY_OS_WINDOWS ) && (!defined( _MSC_VER ) || defined( _MSC_EXTENSIONS))
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
//#define WIN32_WINNT 0x600
#include <windows.h>

#endif

#endif //ndef IG_INDUSTRY_API_WINDOWS_IMPORT
