// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_CANVAS
#define IG_INDUSTRY_API_OPENGL_CANVAS

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/texture.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			class canvas {
				boost::multi_array< texture2d, 2 > textures;
				unsigned width, height;
			public:
				canvas( unsigned width, unsigned height ): width(width), height(height) {
					assert( width && height );
					size_t tex_size = std::max(max_texture_size(),max_rectangular_texture_size());
					size_t texs_w = (width  - 1)/tex_size+1;
					size_t texs_h = (height - 1)/tex_size+1;
					textures.resize( boost::extents[texs_w][texs_h] );

					std::vector< color4ub > blackness( tex_size*tex_size , color4ub(0,0,0,0xFF) );

					for ( size_t y = 0 ; y < texs_h ; ++y ) {
						for ( size_t x = 0 ; x < texs_w ; ++x ) {
							GLuint w = (x != texs_w-1) ? tex_size : width %tex_size;
							GLuint h = (y != texs_h-1) ? tex_size : height%tex_size;

							textures[x][y] = texture2d(w,h,blackness);
						}
					}
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_CANVAS
