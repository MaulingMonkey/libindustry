// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TEXCOORD
#define IG_INDUSTRY_API_OPENGL_TEXCOORD

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/types.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			template < typename T , size_t N > struct texcoord;

			template < typename T > struct texcoord< T, 1 > {
				T u;

				texcoord( T u ): u(u) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
				enum { components = 1 };
			};

			template < typename T > struct texcoord< T, 2 > {
				T u, v;

				texcoord( T u, T v ): u(u), v(v) {}

				typedef T           component_type;
				static const GLenum component_type_enum = detail::type_to_enum< T >::value;
				enum { components = 2 };
			};

			typedef texcoord< GLfloat , 1 > texcoord1f;
			typedef texcoord< GLdouble, 1 > texcoord1d;

			typedef texcoord< GLfloat , 2 > texcoord2f;
			typedef texcoord< GLdouble, 2 > texcoord2d;

			inline void glTexCoord( const texcoord1f & tc ) { ::glTexCoord1f ( tc.u ); }
			inline void glTexCoord( const texcoord1d & tc ) { ::glTexCoord1d ( tc.u ); }

			inline void glTexCoord( const texcoord2f & tc ) { ::glTexCoord2f ( tc.u, tc.v ); }
			inline void glTexCoord( const texcoord2d & tc ) { ::glTexCoord2d ( tc.u, tc.v ); }

			template < typename T , size_t N >
			inline void glTexCoordPointer( const texcoord<T,N> *list ) {
				::glTexCoordPointer( N, texcoord<T,N>::component_type_enum, sizeof(texcoord<T,N>), &(list->u) );
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXCOORD
