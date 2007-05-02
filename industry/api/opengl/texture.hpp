// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TEXTURE
#define IG_INDUSTRY_API_OPENGL_TEXTURE

#include <industry/api/devil/import.hpp>
#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/color.hpp>
#include <industry/api/opengl/extensions.hpp>
#include <industry/api/opengl/exceptions.hpp>
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
			template < size_t N = 0 > class texture;

			namespace detail {
				struct texture_impl : boost::noncopyable {
					texture_impl(): scale_width(1.0f), scale_height(1.0f) { glGenTextures(1,&id); }
					~texture_impl() { glDeleteTextures(1,&id); }

					GLuint id, type;
					GLuint  pixels_width, pixels_height;
					GLfloat scale_width , scale_height;
				};
				typedef boost::shared_ptr< texture_impl > texture_impl_ptr;

				template < typename T > struct is_a_texture               { enum { value = false }; };
				template < size_t N >   struct is_a_texture< texture<N> > { enum { value = true  }; };
			}

			template <> class texture<0> {
			protected:
				detail::texture_impl_ptr impl;
			public:
				friend inline void select( const texture& t ) {
					glBindTexture(t.impl->type,t.impl->id);
					glEnable(t.impl->type);
					GLenum mode;
					glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode ); 
					glMatrixMode( GL_TEXTURE );
					glPushMatrix();
					glScalef( t.impl->scale_width , t.impl->scale_height , 1.0f );
					glMatrixMode( mode );
				}
				friend inline void unselect( const texture& t ) {
					glDisable(t.impl->type);
					GLenum mode;
					glGetIntegerv( GL_MATRIX_MODE , (GLint*)&mode );
					glMatrixMode( GL_TEXTURE );
					glPopMatrix();
					glMatrixMode( mode );
				}
			protected:
				template < typename T > static bool is_pot( T v ) { return !(v & (v-1)); }
				template < typename T > static T upper_pot( T v ) { unsigned n = 1; while(n<v) n *= 2; return n; }

				template < typename T , size_t N > static bool is_pot( const T(&vs)[N] ) {
					for ( unsigned i = 0 ; i < N ; ++i ) if (!is_pot(vs[i])) return false;
					return true;
				}
			};

			template <> class texture<2>: public texture<> {
			public:
				texture() {}
				texture( const std::string& filename ) {
					ilEnable( IL_ORIGIN_SET );
					ilOriginFunc( IL_ORIGIN_LOWER_LEFT );
					if (!ilLoadImage( (const ILstring)filename.c_str() )) throw file_not_found();
					GLuint dimms[] = { GLuint(ilGetInteger( IL_IMAGE_WIDTH )) , GLuint(ilGetInteger( IL_IMAGE_HEIGHT )) };
					if (!is_within_limits(0,dimms)) {
						unsigned limit = std::max( max_texture_size() , max_rectangular_texture_size() );
						iluImageParameter( ILU_FILTER , ILU_SCALE_LANCZOS3 );
						iluScale( limit, limit, 1 );
						dimms[0] = limit;
						dimms[1] = limit;
					}
					do_create(texture_type_for(dimms),dimms,GL_RGBA,GL_UNSIGNED_BYTE,ilGetData());
					iluScale(1,1,1);
				}
				template < typename T >
				texture( const boost::multi_array<T,2>& source ) {
					GLuint dimms[] = { GLuint(source.shape()[0]), GLuint(source.shape()[1]) };
					if (!is_within_limits(0,dimms)) {
						unsigned limit = std::max( max_texture_size() , max_rectangular_texture_size() );
						ilTexImage( dimms[0], dimms[1], 1, T::components, T::format_enum, T::component_type_enum, const_cast< T* >( source.data() ) );
						iluImageParameter( ILU_FILTER , ILU_SCALE_LANCZOS3 );
						iluScale( limit, limit, 1 );
						dimms[0] = limit;
						dimms[1] = limit;
					}
					do_create(texture_type_for(dimms),dimms,T::format_enum,T::component_type_enum,source.data());
					iluScale(1,1,1);
				}
				template < typename T >
				texture( unsigned w, unsigned h , const std::vector<T> & data ) {
					assert( w*h <= data.size() );
					GLuint dimms[] = {w,h};
					do_create(texture_type_for(dimms),dimms,T::format_enum,T::component_type_enum,&data[0]);
				}
				~texture() {
				}

				template < typename T >
				void blit( GLuint x, GLuint y, const boost::multi_array<T,2>& source ) {
					do_blit( x, y, source.shape()[0], source.shape()[1], T::format_enum, T::component_type_enum, source.data() );
				}
			private:
				void do_create( GLenum type , const GLuint (&dimms)[2] , GLenum format, GLenum component, const void* data ) {
					impl.reset( new detail::texture_impl );
					impl->type   = type;
					if (!is_within_limits(impl->type,dimms)) throw texture_size_unavailable();
					bool rects = (impl->type == GL_TEXTURE_RECTANGLE_ARB);
					bool need_resize = !rects && !is_pot(dimms);
					impl->pixels_width  = dimms[0];
					impl->pixels_height = dimms[1];
					impl->scale_width   = rects ? GLfloat(dimms[0]) : (GLfloat(dimms[0]) / upper_pot(dimms[0]));
					impl->scale_height  = rects ? GLfloat(dimms[1]) : (GLfloat(dimms[1]) / upper_pot(dimms[1]));
					glBindTexture  (impl->type,impl->id);
					glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
					if (!need_resize) {
						glTexImage2D(impl->type, 0, 4, dimms[0], dimms[1], 0, format, component, data );
					} else {
						glTexImage2D(impl->type, 0, 4, upper_pot(dimms[0]), upper_pot(dimms[1]), 0, format, component, NULL );
						glTexSubImage2D(impl->type,0,0,0,dimms[0],dimms[1],format,component,data);
					}
					glTexParameteri(impl->type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(impl->type,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(impl->type,GL_TEXTURE_WRAP_S,GL_CLAMP);
					glTexParameteri(impl->type,GL_TEXTURE_WRAP_T,GL_CLAMP);
				}
				void do_blit( GLuint x, GLuint y, GLuint w, GLuint h, GLenum format, GLenum component, const void* data ) {
					assert( x+w <= impl->pixels_width && y+h <= impl->pixels_height && w && h );
					glBindTexture( impl->type , impl->id );
					glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );
					glTexSubImage2D(impl->type,0,x,y,w,h,format,component,data);
				}
				static GLenum texture_type_for( const GLuint (&dimms)[2] ) {
					bool square = dimms[0] == dimms[1];
					bool pot    = is_pot(dimms);

					if ( square && (pot || has_npot_textures()) && is_within_limits(GL_TEXTURE_2D,dimms) ) return GL_TEXTURE_2D;
					if ( has_rectangular_textures() && is_within_limits(GL_TEXTURE_RECTANGLE_ARB,dimms) ) return GL_TEXTURE_RECTANGLE_ARB;
					if ( is_within_limits(GL_TEXTURE_2D,dimms) ) return GL_TEXTURE_2D;
					throw texture_size_unavailable();
				}
				static bool is_within_limits( GLuint type , const GLuint (&dimms)[2] ) {
					switch( type ) {
						case 0:
							return is_within_limits( GL_TEXTURE_2D            , dimms )
								|| is_within_limits( GL_TEXTURE_RECTANGLE_ARB , dimms )
								;
						case GL_TEXTURE_2D:
							return (dimms[0] <= max_texture_size())
								&& (dimms[1] <= max_texture_size())
								;
						case GL_TEXTURE_RECTANGLE_ARB:
							return (dimms[0] <= max_rectangular_texture_size())
								&& (dimms[1] <= max_rectangular_texture_size())
								;
						default:
							assert(!"reached");
							return false;
					}
				}
			};

			typedef texture<1> texture1d;
			typedef texture<2> texture2d;
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
