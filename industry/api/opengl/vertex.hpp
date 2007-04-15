// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_VERTEX
#define IG_INDUSTRY_API_OPENGL_VERTEX

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/types.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			template < typename T , size_t N > struct vertex;

			template < typename T > struct vertex< T, 2 > {
				T x, y;

				vertex( T x, T y ): x(x), y(y) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
				enum { components = 2 };
			};

			template < typename T > struct vertex< T, 3 > {
				T x, y, z;

				vertex( T x, T y, T z ): x(x), y(y), z(z) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
				enum { components = 3 };
			};

			typedef vertex< GLfloat , 2 > vertex2f;
			typedef vertex< GLdouble, 2 > vertex2d;

			typedef vertex< GLfloat , 3 > vertex3f;
			typedef vertex< GLdouble, 3 > vertex3d;

			inline void glVertex( const vertex2f & v ) { ::glVertex2f ( v.x, v.y ); }
			inline void glVertex( const vertex2d & v ) { ::glVertex2d ( v.x, v.y ); }

			inline void glVertex( const vertex3f & v ) { ::glVertex3f ( v.x, v.y, v.z ); }
			inline void glVertex( const vertex3d & v ) { ::glVertex3d ( v.x, v.y, v.z ); }

			template < typename T , size_t N >
			inline void glVertexPointer( const vertex<T,N> *list ) {
				::glVertexPointer( N, vertex<T,N>::component_type_enum, sizeof(vertex<T,N>), &(list->x) );
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_VERTEX
