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
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor.hpp>

#ifndef INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS
#define INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS 10
#endif //ndef INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS

#define DO_DISPLAY_LIST_CTOR(z,n,ynysed)                               \
	template < BOOST_PP_ENUM_PARAMS(n, typename A) >                   \
	display_list( BOOST_PP_ENUM_BINARY_PARAMS(n, A, arg) ) {           \
		config( boost::make_tuple( BOOST_PP_ENUM_PARAMS(n, arg) ) );   \
	}                                                                  \
/*--------------------------------------------------------------------*/

namespace industry {
	namespace api {
		namespace opengl {
			namespace detail {
				struct display_list_id : boost::noncopyable {
					display_list_id()  { id = ::glGenLists(1);  }
					~display_list_id() { ::glDeleteLists(id,1); }

					GLuint id;
				};
			}

			class display_list {
				boost::shared_ptr< detail::display_list_id > id;
			public:
				display_list() {}
				BOOST_PP_REPEAT_FROM_TO(1,INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS,DO_DISPLAY_LIST_CTOR,~)
				~display_list() {}

				template < typename Tuple >
				void config( const Tuple& params ) {
				}

				friend void glNewList( const display_list& list , GLenum mode ) {
					::glNewList( list.id->id , mode );
				}
				//friend void glEndList(); //No override needed, OpenGL's does everything we need.
				friend void glCallList( const display_list& list ) {
					::glCallList( list.id->id );
				}
			};
		}
	}
}

#undef DO_DISPLAY_LIST_CTOR

#endif //ndef IG_INDUSTRY_API_OPENGL_DISPLAY_LIST
