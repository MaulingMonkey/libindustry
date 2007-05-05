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
				canvas( unsigned width, unsigned height ) {
					do_create(width,height);
				}
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
				void blit( unsigned xoffset , unsigned yoffset , unsigned width, unsigned height, const C* data ) {
					int texs_w = int(textures.shape()[0]);
					int texs_h = int(textures.shape()[1]);
					for ( int tex_y = 0 ; tex_y < texs_h ; ++tex_y ) {
						for ( int tex_x = 0 ; tex_x < texs_w ; ++tex_x ) {
							int tex_w = (tex_x != texs_w-1) ? size_mod : (this->width -1)%size_mod+1;
							int tex_h = (tex_y != texs_h-1) ? size_mod : (this->height-1)%size_mod+1;

							int tex_xoff = tex_x*size_mod;
							int tex_yoff = tex_y*size_mod;
							int local_xoff = int(xoffset) - tex_xoff;
							int local_yoff = int(yoffset) - tex_yoff;

							int left   = std::max< int >( 0                   , local_xoff );
							int right  = std::min< int >( local_xoff + width  , tex_w      );
							int top    = std::max< int >( 0                   , local_yoff );
							int bottom = std::min< int >( local_yoff + height , tex_h      );

							if ( left >= right ) continue;
							if ( top >= bottom ) continue;

							int glob_width  = right-left;
							int glob_height = bottom-top;

							if ( glob_width == width || glob_height == height ) {
								// glob to blit is same dimensions as data already is, skip subdividing entirely...
								textures[tex_x][tex_y].blit(left,top,width,height,data);
							} else {
								boost::multi_array< C, 2 > glob( boost::extents[glob_width][glob_height] , boost::fortran_storage_order() );

								for ( int y = top ; y != bottom ; ++y ) {
									for ( int x = left ; x != right ; ++x ) {
										int data_x = x + tex_xoff - xoffset;
										int data_y = y + tex_yoff - yoffset;

										glob[x-left][y-top] = data[ data_y*width + data_x ];
									}
								}

								textures[tex_x][tex_y].blit(left,top,glob);
							}
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

					std::vector< color4ub > blackness;
					blackness.resize( size_mod*size_mod , color4ub(0,0,0,0xFF) );
					for ( size_t y = 0 ; y < texs_h ; ++y ) {
						for ( size_t x = 0 ; x < texs_w ; ++x ) {
							GLuint w = (x != texs_w-1) ? size_mod : (width -1)%size_mod+1;
							GLuint h = (y != texs_h-1) ? size_mod : (height-1)%size_mod+1;

							textures[x][y] = texture2d(w,h,blackness);
						}
					}

					display_list_compiler compile;
					for ( size_t y = 0 ; y < texs_h ; ++y ) {
						for ( size_t x = 0 ; x < texs_w ; ++x ) {
							GLuint w = (x != texs_w-1) ? size_mod : (width -1)%size_mod+1;
							GLuint h = (y != texs_h-1) ? size_mod : (height-1)%size_mod+1;

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
