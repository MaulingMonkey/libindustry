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
			template < size_t N , typename Tag = nil > class texture;

			//  Tag          Programmatically unavailable (always asserted) options:
			//  minimal      empty, Oversized
			//  nil          empty, non-pow2, non-rectangular
			//  rectangular  empty, mipmapping, many clamp modes, etc
			//  non_pow2     empty

			//  Tag          Checked options (thrown if unavailable):
			//  minimal      *nothing*
			//  nil          Oversized
			//  rectangular  Oversized, non-pow2
			//  non_pow2     Oversized, non-pow2, lack of clamp mode in non-pow2, etc



			namespace detail {
				struct texture_data : boost::noncopyable {
					texture_data(): width(1), height(1) { glGenTextures(1,&id); }
					~texture_data() { glDeleteTextures(1,&id); }

					GLuint id;
					GLuint width, height;
				};

				template < typename T >              struct is_a_texture                   { enum { value = false }; };
				template < size_t N , typename Tag > struct is_a_texture< texture<N,Tag> > { enum { value = true  }; };

				template < typename Tag > struct verify_texture_preconditions;

				template <> struct verify_texture_preconditions< minimal > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                          ); // not empty
							assert( data.shape()[i] == data.shape()[0]       ); // rectangular
							assert( data.shape()[i] <= 64                    ); // right size
							assert( !(data.shape()[i] & (data.shape()[i]-1)) ); // power of 2
						}
					}
				};

				template <> struct verify_texture_preconditions< nil > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                          ); // not empty
							assert( data.shape()[i] == data.shape()[0]       ); // rectangular
							if    ( data.shape()[i] > max_texture_size()     ) throw texture_size_unavailable();
							assert( !(data.shape()[i] & (data.shape()[i]-1)) ); // power of 2
						}
					}
				};

				template <> struct verify_texture_preconditions< rectangular > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                                                        ); // not empty
							if    ( data.shape()[i] != data.shape()[0] && !has_rectangular_textures()      ) throw texture_shape_unavailable();
							if    ( data.shape()[i] > max_rectangular_texture_size()                       ) throw texture_size_unavailable();
							if    ( (data.shape()[i] & (data.shape()[i]-1)) && !has_rectangular_textures() ) throw texture_shape_unavailable();
						}
					}
				};

				template <> struct verify_texture_preconditions< non_pow2 > {
					template < typename T > static void on( const boost::multi_array< T, 2 > & data ) {
						for ( unsigned i = 0 ; i < 2 ; ++i ) {
							assert( data.shape()[i]                                                     ); // not empty
							if    ( data.shape()[i] != data.shape()[0] && !has_non_pow2_textures()      ) throw texture_shape_unavailable();
							if    ( data.shape()[i] > max_texture_size()                                ) throw texture_size_unavailable();
							if    ( (data.shape()[i] & (data.shape()[i]-1)) && !has_non_pow2_textures() ) throw texture_shape_unavailable();
						}
					}
				};
			}
			
			template < typename Tag > class texture<2,Tag> {
				typedef detail::texture_data  texture_data;
				boost::shared_ptr< texture_data > data;
			public:
				friend inline void glBindTexture( const texture& t ) { ::glBindTexture( t.target_enum(), t.data->id ); }
				friend inline void select( const texture& t ) {
					glBindTexture(t);
					glEnable(t.target_enum());
					GLenum mode;
					glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode ); 
					glMatrixMode( GL_TEXTURE );
					glPushMatrix();
					glScalef( 1.0f * t.data->width , 1.0f * t.data->height , 1.0f );
					glMatrixMode( mode );
				}
				friend inline void unselect( const texture& t ) {
					glDisable(t.target_enum());
					GLenum mode;
					glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode );
					glMatrixMode( GL_TEXTURE );
					glPopMatrix();
					glMatrixMode( mode );
				}

				texture() {}

				template < typename T >
				texture( const boost::multi_array<T,2>& source ) {
					detail::verify_texture_preconditions< Tag >::on( source );

					data.reset( new texture_data );
					glBindTexture( *this );
					glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
					glTexImage2D( target_enum(), 0, 3, static_cast<GLsizei>(source.shape()[0]), static_cast<GLsizei>(source.shape()[1]), 0, T::format_enum, T::component_type_enum, source.data() );
					glTexParameteri(target_enum(),GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(target_enum(),GL_TEXTURE_MAG_FILTER,GL_LINEAR);

					if ( target_enum() == GL_TEXTURE_RECTANGLE_ARB ) {
						data->width  = source.shape()[0];
						data->height = source.shape()[1];
					}
				}
				~texture() {
				}

				GLenum target_enum() const {
					return (boost::is_same< Tag , rectangular >::value && has_rectangular_textures()) ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
				}
			};

			typedef texture<1> texture1d;
			typedef texture<2> texture2d;
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
