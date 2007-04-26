// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TYPES
#define IG_INDUSTRY_API_OPENGL_TYPES

#include <industry/api/opengl/import.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			namespace detail {
				template < typename T > struct type_to_enum;
				template <> struct type_to_enum< GLint    > { static const GLenum value = GL_INT;           };
				template <> struct type_to_enum< GLfloat  > { static const GLenum value = GL_FLOAT;         };
				template <> struct type_to_enum< GLdouble > { static const GLenum value = GL_DOUBLE;        };
				template <> struct type_to_enum< GLubyte  > { static const GLenum value = GL_UNSIGNED_BYTE; };
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TYPES
