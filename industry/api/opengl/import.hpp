// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_IMPORT
#define IG_INDUSTRY_API_OPENGL_IMPORT

#include <industry/config.hpp>
#ifdef INDUSTRY_OS_WINDOWS
#include <industry/api/windows/import.hpp>
#endif

#if   defined( __APPLE__ )
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif //ndef IG_INDUSTRY_API_OPENGL_IMPORT
