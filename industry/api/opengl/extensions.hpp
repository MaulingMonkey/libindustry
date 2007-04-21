// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_EXTENSIONS
#define IG_INDUSTRY_API_OPENGL_EXTENSIONS

#include <industry/api/opengl/import.hpp>
#include <boost/tokenizer.hpp>
#include <set>
#include <string>

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB            0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB    0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB      0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB   0x84F8
#define GL_SAMPLER_2D_RECT_ARB              0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB       0x8B64
#endif

// Testing options:
// #define INDUSTRY_API_OPENGL_DISABLE_ALL_EXTENSIONS
// #define INDUSTRY_API_OPENGL_DISABLE_RECTANGULAR_TEXTURES
// #define INDUSTRY_API_OPENGL_DISABLE_NPOT_TEXTURES
// #define INDUSTRY_API_OPENGL_OVERRIDE_MAX_TEXTURE_SIZE              64   // [ unsigned int ]
// #define INDUSTRY_API_OPENGL_OVERRIDE_MAX_RECTANGULAR_TEXTURE_SIZE  64   // [ unsigned int ]

namespace industry {
	namespace api {
		namespace opengl {
			inline std::set< std::string > available_extensions() {
				const bool cache_extensions = false;

				std::set< std::string > extensions;
				if ( extensions.empty() || !cache_extensions ) {
					extensions.clear();
					const char * string = (const char *) glGetString( GL_EXTENSIONS );
					while ( *string ) {
						const char * i = string;
						while( *i && *i != ' ' ) ++i;
						extensions.insert( std::string(string, i) );
						string = i;
						while ( *string == ' ' ) ++string;
					}
				}
#ifndef INDUSTRY_API_OPENGL_DISABLE_ALL_EXTENSIONS
				return extensions;
#else
				return std::set<std::string>();
#endif
			}
			inline bool has_extension( const std::string & e ) {
				std::set< std::string > es = available_extensions();
				bool has = es.find(e) != es.end();
#ifndef INDUSTRY_API_OPENGL_DISABLE_ALL_EXTENSIONS
				return has;
#else
				return false;
#endif
			}
#ifndef INDUSTRY_API_OPENGL_DISABLE_RECTANGULAR_TEXTURES
			inline bool has_rectangular_textures()       { return has_extension( "GL_ARB_texture_rectangle" ) || has_extension( "GL_EXT_texture_rectangle" ); }
#else
			inline bool has_rectangular_textures()       { return false; }
#endif
#ifndef INDUSTRY_API_OPENGL_DISABLE_NPOT_TEXTURES
			inline bool has_npot_textures()              { return has_extension( "GL_ARB_texture_non_power_of_two" ) || has_extension( "GL_EXT_texture_non_power_of_two" ); }
#else
			inline bool has_npot_textures()              { return false; }
#endif
#if !defined( INDUSTRY_API_OPENGL_OVERRIDE_MAX_TEXTURE_SIZE )
			inline size_t max_texture_size()             { GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE              , &texSize); return texSize; }
#else
			inline size_t max_texture_size()             { GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE              , &texSize); return std::min(texSize,INDUSTRY_API_OPENGL_OVERRIDE_MAX_TEXTURE_SIZE); }
#endif
#if !defined( INDUSTRY_API_OPENGL_OVERRIDE_MAX_RECTANGULAR_TEXTURE_SIZE )
			inline size_t max_rectangular_texture_size() { GLint texSize; glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize); return texSize; }
#else
			inline size_t max_rectangular_texture_size() { GLint texSize; glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize); return std::min(texSize,INDUSTRY_API_OPENGL_OVERRIDE_MAX_RECTANGULAR_TEXTURE_SIZE); }
#endif
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_EXTENSIONS
