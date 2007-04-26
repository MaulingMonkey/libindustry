// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_CANVAS
#define IG_INDUSTRY_API_OPENGL_CANVAS

#include <industry/api/devil/import.hpp>
#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/display_list.hpp>
#include <industry/api/opengl/texture.hpp>
#include <industry/pod/tuple.hpp>
#include <boost/multi_array.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace opengl {
			class canvas {
				boost::multi_array< texture2d, 2 > textures;
				display_list list;
				GLuint width, height, size_mod;
			public:
				void render() const { glCallList(list); }
				template < typename C >
				canvas( const boost::multi_array< C, 2 > & data ) {
					do_create(data.shape()[0],data.shape()[1]);
					blit(0,0,data);
				}
				canvas( const std::string& filename ) {
					ilEnable( IL_ORIGIN_SET );
					ilOriginFunc( IL_ORIGIN_LOWER_LEFT );
					if (!ilLoadImage( (const ILstring)filename.c_str() )) throw file_not_found();
					ILuint dimms[] = { ilGetInteger( IL_IMAGE_WIDTH ) , ilGetInteger( IL_IMAGE_HEIGHT ) };
					do_create(dimms[0],dimms[1]);
					blit(0,0,dimms[0],dimms[1],(const color4ub*)ilGetData());
				}
				template < typename C >
				void blit( unsigned x, unsigned y, const boost::multi_array< C, 2 > & data ) {
					blit(x,y,data.shape()[0],data.shape()[1],data.data());
				}
				template < typename C >
				void blit( unsigned sx , unsigned sy , unsigned sw, unsigned sh, const C* data ) {
					int texs_w = int(textures.shape()[0]);
					int texs_h = int(textures.shape()[1]);
					for ( int ty = 0 ; ty < texs_h ; ++ty ) {
						for ( int tx = 0 ; tx < texs_w ; ++tx ) {
							int tex_w = (tx != texs_w-1) ? size_mod : width %size_mod;
							int tex_h = (ty != texs_h-1) ? size_mod : height%size_mod;

							int target_left   = std::max<int>( 0    , sx    - tx*size_mod );
							int target_right  = std::min<int>( tex_w, sx+sw - tx*size_mod );
							int target_top    = std::max<int>( 0    , sy    - ty*size_mod );
							int target_bottom = std::min<int>( tex_h, sy+sh - ty*size_mod );

							if ( target_left == target_right  ) continue;
							if ( target_top  == target_bottom ) continue;

							GLuint target_width  = target_right  - target_left;
							GLuint target_height = target_bottom - target_top ;

							boost::multi_array< C, 2 > glob( boost::extents[target_width][target_height] );
							int source_left = target_left + tx*size_mod;
							int source_top  = target_top  + ty*size_mod;

							for ( unsigned y = 0 ; y < target_height ; ++y ) {
								std::copy( data + (source_top+y)*sw + source_left , data + (source_top+y)*(sw) + source_left + target_width , glob.data() + (target_top+y)*(target_width) );
							}
							
							textures[tx][ty].blit(target_left,target_top,glob);
						}
					}
				}
			private:
				void do_create( unsigned width, unsigned height ) {
					this->width  = width;
					this->height = height;
					assert( width && height );
					size_mod = std::max(max_texture_size(),max_rectangular_texture_size());
					size_t texs_w = (width  - 1)/size_mod+1;
					size_t texs_h = (height - 1)/size_mod+1;
					textures.resize( boost::extents[texs_w][texs_h] );

					std::vector< color4ub > blackness( size_mod*size_mod , color4ub(0,0,0,0xFF) );
					for ( size_t y = 0 ; y < texs_h ; ++y ) {
						for ( size_t x = 0 ; x < texs_w ; ++x ) {
							GLuint w = (x != texs_w-1) ? size_mod : width %size_mod;
							GLuint h = (y != texs_h-1) ? size_mod : height%size_mod;

							textures[x][y] = texture2d(w,h,blackness);
						}
					}

					display_list_compiler compile;
					for ( size_t y = 0 ; y < texs_h ; ++y ) {
						for ( size_t x = 0 ; x < texs_w ; ++x ) {
							GLuint w = (x != texs_w-1) ? size_mod : width %size_mod;
							GLuint h = (y != texs_h-1) ? size_mod : height%size_mod;

							pod::tuple< texcoord2f , vertex2i > data[] = {
								{ 0.0f, 0.0f, x*size_mod  , y*size_mod   },
								{ 0.0f, 1.0f, x*size_mod  , y*size_mod+h },
								{ 1.0f, 1.0f, x*size_mod+w, y*size_mod+h },
								{ 1.0f, 0.0f, x*size_mod+w, y*size_mod   },
							};
							compile( GL_QUADS, data, textures[x][y] );
						}
					}
					list = compile;
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_CANVAS
