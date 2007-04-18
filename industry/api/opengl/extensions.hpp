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
				return extensions;
			}
			inline bool has_extension( const std::string & e ) {
				std::set< std::string > es = available_extensions();
				bool has = es.find(e) != es.end();
				return has;
			}
			inline bool has_rectangular_textures()       { return has_extension( "GL_ARB_texture_rectangle" ) || has_extension( "GL_EXT_texture_rectangle" ); }
			inline bool has_non_pow2_textures()          { return has_extension( "GL_ARB_texture_non_power_of_two" ); }
			inline size_t max_texture_size()             { GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE              , &texSize); return texSize; }
			inline size_t max_rectangular_texture_size() { GLint texSize; glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize); return texSize; }
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_EXTENSIONS
