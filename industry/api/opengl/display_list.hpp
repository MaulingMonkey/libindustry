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
#include <industry/api/opengl/color.hpp>
#include <industry/api/opengl/texcoord.hpp>
#include <industry/api/opengl/texture.hpp>
#include <industry/api/opengl/vertex.hpp>
#include <industry/utility/tuple_accessors.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor.hpp>

#ifndef INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS
#define INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS 10
#endif //ndef INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS

/*---------------------------------------------------------------------------------*/
#define DO_DISPLAY_LIST_CTOR(z,n,ynysed)                                            \
	template < typename D , size_t N  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename P) > \
	display_list( GLenum mode , const D (&data)[N]                                  \
		BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n,P,param) )                           \
	{                                                                               \
		*this = detail::display_list_compiler()                                     \
			( mode , data  BOOST_PP_ENUM_TRAILING_PARAMS(n,param) );                \
	}                                                                               \
/*---------------------------------------------------------------------------------*/
#define DO_DISPLAY_LIST_FROM(z,n,unused)                                            \
	template < typename D , size_t N  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename P) > \
	static detail::display_list_compiler from( GLenum mode , const D (&data)[N]     \
		BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n,P,param) )                           \
	{                                                                               \
		return detail::display_list_compiler()                                      \
			( mode , data  BOOST_PP_ENUM_TRAILING_PARAMS(n,param) );                \
	}                                                                               \
/*---------------------------------------------------------------------------------*/
#define DO_DISPLAY_LIST_SELECT(z,n,param)                                           \
	do_select( BOOST_PP_CAT(param,n) );                                             \
/*---------------------------------------------------------------------------------*/
#define DO_DISPLAY_LIST_COMPILER_OPER(z,n,unused)                                   \
	template < typename D , size_t N  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename P) > \
	display_list_compiler operator()( GLenum mode, const D (&data)[N]               \
		BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(n,P,param) )                           \
	{                                                                               \
		BOOST_PP_REPEAT(n,DO_DISPLAY_LIST_SELECT,param)                             \
		do_compile(mode,data);                                                      \
		return *this;                                                               \
	}                                                                               \
/*---------------------------------------------------------------------------------*/

namespace industry {
	namespace api {
		namespace opengl {
			class display_list;

			namespace detail {
				struct display_list_id : boost::noncopyable {
					display_list_id()  { id = ::glGenLists(1);  }
					~display_list_id() { ::glDeleteLists(id,1); }

					GLuint id;
				};
				template < typename T > struct is_a_display_list     { enum { value = false }; };
				template <> struct is_a_display_list< display_list > { enum { value = true  }; };

				class display_list_compiler {
					friend class display_list;
					mutable boost::shared_ptr< display_list_id > id;
				public:
					display_list_compiler() {
						id.reset( new display_list_id );
						glNewList( id->id , GL_COMPILE );
					}
					display_list_compiler( const display_list_compiler& other ): id(other.id) {
						other.id.reset();
					}
					~display_list_compiler() {
						if (id) glEndList();
					}

					void do_select ( const texture<2>& texture ) { glBindTexture(texture); glEnable( GL_TEXTURE_2D ); }

					template < typename Tuple , size_t N >
					void do_compile( GLenum mode , const Tuple (&data)[N] ) {
						glBegin( mode );
						for ( unsigned i = 0 ; i < N ; ++i ) {
							using namespace industry::utility;
							glColor   ( get_if_type_or_default< is_a_color    >(data[i].unpod(),industry::nil()) );
							glTexCoord( get_if_type_or_default< is_a_texcoord >(data[i].unpod(),industry::nil()) );
							glVertex  ( get_if_type           < is_a_vertex   >(data[i].unpod()) );
						}
						glEnd();
					}

					BOOST_PP_REPEAT_FROM_TO( 0 , INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS , DO_DISPLAY_LIST_COMPILER_OPER , ~ )
				};
			}

			class display_list {
				boost::shared_ptr< detail::display_list_id > id;
			public:
				display_list() {}
				display_list( const detail::display_list_compiler& compiler ): id(compiler.id) {}
				BOOST_PP_REPEAT_FROM_TO(0,INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS,DO_DISPLAY_LIST_CTOR,~)
				BOOST_PP_REPEAT_FROM_TO(0,INDUSTRY_API_OPENGL_DISPLAY_LIST_MAX_ARGS,DO_DISPLAY_LIST_FROM,~)
				~display_list() {}

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
#undef DO_DISPLAY_LIST_FROM
#undef DO_DISPLAY_LIST_SELECT
#undef DO_DISPLAY_LIST_COMPILER_OPER

#endif //ndef IG_INDUSTRY_API_OPENGL_DISPLAY_LIST
