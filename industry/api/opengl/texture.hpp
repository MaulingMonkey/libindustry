// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TEXTURE
#define IG_INDUSTRY_API_OPENGL_TEXTURE

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/color.hpp>
#include <industry/api/opengl/extensions.hpp>
#include <industry/api/opengl/exceptions.hpp>
#include <industry/api/opengl/tags.hpp>
#include <industry/api/opengl/types.hpp>
#include <industry/nil.hpp>
#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <cassert>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4267 )
#endif
#include <boost/multi_array.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace industry {
	namespace api {
		namespace opengl {
			// Note:  texture<> is NOT necessairly normalized, only texture<N,normalized> for N != 0 is.  For now.
			template < size_t N = 0 , typename Tag = normalized > class texture;

			namespace detail {
				struct texture_impl : boost::noncopyable {
					texture_impl(): width(1), height(1) { glGenTextures(1,&id); }
					texture_impl( GLuint id ): id(id), width(1), height(1) {}
					~texture_impl() { glDeleteTextures(1,&id); }

					GLuint id;
					GLuint type;
					GLuint width, height;
				};

				template < typename T >              struct is_a_texture                   { enum { value = false }; };
				template < size_t N , typename Tag > struct is_a_texture< texture<N,Tag> > { enum { value = true  }; };

				template < typename Tag > struct verify_texture_preconditions;

				template <> struct verify_texture_preconditions< unnormalized > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						const bool npot = has_rectangular_textures() || has_npot_textures();

						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                                  ); // not empty
							if    ( data.shape()[i] != data.shape()[0] && !npot      ) throw texture_shape_unavailable();
							if    ( data.shape()[i] > max_rectangular_texture_size() ) throw texture_size_unavailable();
							if    ( (data.shape()[i] & (data.shape()[i]-1)) && !npot ) throw texture_shape_unavailable();
						}
					}
				};

				template <> struct verify_texture_preconditions< normalized > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						const bool npot = has_npot_textures();

						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                                  ); // not empty
							if    ( data.shape()[i] != data.shape()[0] && !npot      ) throw texture_shape_unavailable();
							if    ( data.shape()[i] > max_texture_size()             ) throw texture_size_unavailable();
							if    ( (data.shape()[i] & (data.shape()[i]-1)) && !npot ) throw texture_shape_unavailable();
						}
					}
				};
			}

			// NOTE:  This isn't necessairly a normalized texture (and certainly isn't 0 dimensions).
			//        This is just the base interface (e.g. texture<>) which all texture<N,Tag> should be convertable to.
			template <> class texture<0,normalized> {
			protected:
				typedef detail::texture_impl  texture_impl;
				boost::shared_ptr< texture_impl > impl;
			public:
				friend inline void glBindTexture( const texture& t ) { ::glBindTexture( t.impl->type, t.impl->id ); }

				friend inline void select( const texture& t ) {
					glBindTexture(t);
					glEnable(t.impl->type);
					if ( t.impl->type == GL_TEXTURE_RECTANGLE_ARB ) {
						GLenum mode;
						glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode ); 
						glMatrixMode( GL_TEXTURE );
						glPushMatrix();
						glScalef( 1.0f * t.impl->width , 1.0f * t.impl->height , 1.0f );
						glMatrixMode( mode );
					}
				}
				friend inline void unselect( const texture& t ) {
					glDisable(t.impl->type);
					if ( t.impl->type == GL_TEXTURE_RECTANGLE_ARB ) {
						GLenum mode;
						glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode );
						glMatrixMode( GL_TEXTURE );
						glPopMatrix();
						glMatrixMode( mode );
					}
				}
			};

			template < typename Tag > class texture<2,Tag>: public texture<> {
			public:
				texture() {}
				texture( const texture<2,normalized>& o ) { impl = o.impl; } //allow normalized -> * (e.g. unnormalized) texture conversion
				texture( const api::devil::image& image ) {
					//FIXME:  Error checking please!!!
					//        Merge refactoring too probably?
					//        VTEC just kick in yo

					ilBindImage( image );
					impl.reset( new texture_impl( ilutGLBindTexImage() ) );
					impl->type = GL_TEXTURE_2D; //I think DevIL only uses this?
					impl->width  = ilGetInteger(IL_IMAGE_WIDTH );
					impl->height = ilGetInteger(IL_IMAGE_HEIGHT);
					assert( impl->id != 0 );
					//glBindTexture( *this );
					//glTexParameteri(impl->type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					//glTexParameteri(impl->type,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				}

				template < typename T >
				texture( const boost::multi_array<T,2>& source ) {
					detail::verify_texture_preconditions< Tag >::on( source );

					impl.reset( new texture_impl );
					impl->type = new_texture_type();

					glBindTexture( *this );
					glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
					glTexImage2D   (impl->type, 0, 3, static_cast<GLsizei>(source.shape()[0]), static_cast<GLsizei>(source.shape()[1]), 0, T::format_enum, T::component_type_enum, source.data() );
					glTexParameteri(impl->type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(impl->type,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

					if ( impl->type == GL_TEXTURE_RECTANGLE_ARB ) {
						impl->width  = source.shape()[0];
						impl->height = source.shape()[1];
					}
				}
				~texture() {
				}

				static GLenum new_texture_type() {
					return (boost::is_same< Tag , unnormalized >::value && has_rectangular_textures() && !has_npot_textures()) ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
				}
			};

			typedef texture<1> texture1d;
			typedef texture<2> texture2d;
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
