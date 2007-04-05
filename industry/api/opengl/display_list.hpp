// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_DISPLAY_LIST
#define IG_INDUSTRY_API_OPENGL_DISPLAY_LIST

#include <industry/api/opengl/import.hpp>
#include <boost/noncopyable.hpp>

namespace industry {
	namespace api {
		namespace opengl {
			class display_list_compiler {
				GLuint id;
			public:
				display_list_compiler( GLuint id ): id(id) {
					::glNewList( id , GL_COMPILE );
				}
				display_list_compiler( display_list_compiler& other ): id(other.id) {
					other.id = 0;
				}
				~display_list_compiler() {
					if (id != 0) ::glEndList();
				}
			};


			class display_list : public boost::noncopyable {
				GLuint id;
			public:
				display_list() { id = ::glGenLists(1); }
				~display_list() { ::glDeleteLists(id,1); }

				friend void glNewList( const display_list& list , GLenum mode ) {
					::glNewList( list.id , mode );
				}
				friend display_list_compiler compile( const display_list& list ) {
					return display_list_compiler( list.id );
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_DISPLAY_LIST
