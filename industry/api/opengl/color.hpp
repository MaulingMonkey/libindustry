// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_COLOR
#define IG_INDUSTRY_API_OPENGL_COLOR

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/types.hpp>
#include <industry/graphics/color.hpp>
#include <industry/nil.hpp>
#include <boost/array.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			template < typename T , unsigned N > struct color;

			namespace detail {
				template < typename T >  struct is_a_color                           { enum { value = false }; };
				template < typename T >  struct is_a_color< graphics::rgb      <T> > { enum { value = true  }; };
				template < typename T >  struct is_a_color< graphics::rgba     <T> > { enum { value = true  }; };
				template < typename T >  struct is_a_color< graphics::greyscale<T> > { enum { value = true  }; };

				template < typename T > inline GLenum get_format_of( const graphics::rgb      <T>& ) { return GL_RGB;  }
				template < typename T > inline GLenum get_format_of( const graphics::rgba     <T>& ) { return GL_RGBA; }
				template < typename T > inline GLenum get_format_of( const graphics::greyscale<T>& ) { return GL_LUMINANCE; }
				template < typename T > inline GLenum get_format_of( const T& ) { return get_format_of( typename T::interface_color_type() ); }

				template < typename T > inline GLuint get_components_of( const graphics::rgb      <T>& ) { return 3; }
				template < typename T > inline GLuint get_components_of( const graphics::rgba     <T>& ) { return 4; }
				template < typename T > inline GLuint get_components_of( const graphics::greyscale<T>& ) { return 1; }
				template < typename T > inline GLuint get_components_of( const T& ) { return get_components_of( typename T::interface_color_type() ); }

				template < template < typename > class TT > inline GLenum get_type_of( const TT<unsigned char>& ) { return GL_UNSIGNED_BYTE; }
				template < template < typename > class TT > inline GLenum get_type_of( const TT<float>        & ) { return GL_FLOAT; }
				template < template < typename > class TT > inline GLenum get_type_of( const TT<double>       & ) { return GL_DOUBLE; }
			}

			typedef graphics::greyscale< GLfloat  > color1f;
			typedef graphics::greyscale< GLdouble > color1d;
			typedef graphics::greyscale< GLubyte  > color1ub;

			typedef graphics::rgb< GLfloat  >       color3f;
			typedef graphics::rgb< GLdouble >       color3d;
			typedef graphics::rgb< GLubyte  >       color3ub;

			typedef graphics::rgba< GLfloat  >      color4f;
			typedef graphics::rgba< GLdouble >      color4d;
			typedef graphics::rgba< GLubyte  >      color4ub;

			inline void glColor( const color1f & c ) { ::glColor3f ( c.grey, c.grey, c.grey ); }
			inline void glColor( const color1d & c ) { ::glColor3d ( c.grey, c.grey, c.grey ); }
			inline void glColor( const color1ub& c ) { ::glColor3ub( c.grey, c.grey, c.grey ); }

			inline void glColor( const color3f & c ) { ::glColor3f ( c.red, c.green, c.blue ); }
			inline void glColor( const color3d & c ) { ::glColor3d ( c.red, c.green, c.blue ); }
			inline void glColor( const color3ub& c ) { ::glColor3ub( c.red, c.green, c.blue ); }

			inline void glColor( const color4f & c ) { ::glColor4f ( c.red, c.green, c.blue, c.alpha ); }
			inline void glColor( const color4d & c ) { ::glColor4d ( c.red, c.green, c.blue, c.alpha ); }
			inline void glColor( const color4ub& c ) { ::glColor4ub( c.red, c.green, c.blue, c.alpha ); }

			inline void glColor( industry::nil ) {}

			inline void glClearColor( const color1f & c ) { ::glClearColor( (GLclampf)c.grey, (GLclampf)c.grey, (GLclampf)c.grey, 0.0f); }
			inline void glClearColor( const color1d & c ) { ::glClearColor( (GLclampf)c.grey, (GLclampf)c.grey, (GLclampf)c.grey, 0.0f); }
			inline void glClearColor( const color1ub& c ) { ::glClearColor( 255.0f *  c.grey, 255.0f *  c.grey, 255.0f *  c.grey, 0.0f); }

			inline void glClearColor( const color3f & c ) { ::glClearColor( (GLclampf)c.red, (GLclampf)c.green, (GLclampf)c.blue, 0.0f); }
			inline void glClearColor( const color3d & c ) { ::glClearColor( (GLclampf)c.red, (GLclampf)c.green, (GLclampf)c.blue, 0.0f); }
			inline void glClearColor( const color3ub& c ) { ::glClearColor( 255.0f *  c.red, 255.0f *  c.green, 255.0f *  c.blue, 0.0f); }

			inline void glClearColor( const color4f & c ) { ::glClearColor( (GLclampf)c.red, (GLclampf)c.green, (GLclampf)c.blue, (GLclampf)c.alpha ); }
			inline void glClearColor( const color4d & c ) { ::glClearColor( (GLclampf)c.red, (GLclampf)c.green, (GLclampf)c.blue, (GLclampf)c.alpha ); }
			inline void glClearColor( const color4ub& c ) { ::glClearColor( 255.0f *  c.red, 255.0f *  c.green, 255.0f *  c.blue, 255.0f *  c.alpha ); }

			template < typename T > inline void glClearColor( const T& c ) { glClearColor( typename T::interface_color_type(c) ); }

			template < typename T > inline void glColorPointer( const T* c ) {
				::glColorPointer( detail::get_components_of(T()), detail::get_type_of(T()), sizeof(T), c );
			}

			inline void glColorPointer( industry::nil ) {}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_COLOR
