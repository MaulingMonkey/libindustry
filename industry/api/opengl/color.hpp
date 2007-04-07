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

namespace industry {
	namespace api {
		namespace opengl {
			template < typename T , unsigned N > struct color;
			template < typename T > struct color< T, 3 > {
				T red, green, blue;
				color( T red, T green, T blue ): red(red), green(green), blue(blue) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
			};

			template < typename T > struct color< T, 4 > {
				T red, green, blue, alpha;
				color( T red, T green, T blue, T alpha ): red(red), green(green), blue(blue), alpha(alpha) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
			};

			typedef color< GLfloat , 3 > color3f;
			typedef color< GLdouble, 3 > color3d;
			typedef color< GLubyte , 3 > color3ub;

			typedef color< GLfloat , 4 > color4f;
			typedef color< GLdouble, 4 > color4d;
			typedef color< GLubyte , 4 > color4ub;

			inline void glColor( const color3f & c ) { ::glColor3f ( c.red, c.green, c.blue ); }
			inline void glColor( const color3d & c ) { ::glColor3d ( c.red, c.green, c.blue ); }
			inline void glColor( const color3ub& c ) { ::glColor3ub( c.red, c.green, c.blue ); }

			inline void glColor( const color4f & c ) { ::glColor4f ( c.red, c.green, c.blue, c.alpha ); }
			inline void glColor( const color4d & c ) { ::glColor4d ( c.red, c.green, c.blue, c.alpha ); }
			inline void glColor( const color4ub& c ) { ::glColor4ub( c.red, c.green, c.blue, c.alpha ); }

			template < typename T > inline void glClearColor( const color< T, 3 >& c ) {
				::glClearColor( static_cast<GLdouble>(c.red), static_cast<GLdouble>(c.green), static_cast<GLdouble>(c.blue), 0.0 );
			}
			template < typename T > inline void glClearColor( const color< T, 4 >& c ) {
				::glClearColor( static_cast<GLdouble>(c.red), static_cast<GLdouble>(c.green), static_cast<GLdouble>(c.blue), static_cast<GLdouble>(c.alpha) );
			}

			template < typename T , size_t N >
			inline void glColorPointer( const color<T,N> *list ) {
				::glColorPointer( N, color<T,N>::component_type_enum, sizeof(color<T,N>), &(list->x) );
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_COLOR
