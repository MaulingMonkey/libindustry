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
#include <industry/api/opengl/types.hpp>
#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
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
			namespace detail {
				template < size_t N > struct texture_dims_to_enum;
				template <> struct texture_dims_to_enum<1> { static const GLenum value = GL_TEXTURE_1D; };
				template <> struct texture_dims_to_enum<2> { static const GLenum value = GL_TEXTURE_2D; };
				//template <> struct texture_dims_to_enum<3> { static const GLenum value = GL_TEXTURE_3D; }
			}

			template < size_t N > class texture;
			
			template <> class texture<2>: boost::noncopyable {
				GLuint id;
			public:
				template < typename T >
				texture( const boost::multi_array<T,2>& data ) {
					for ( unsigned i = 0 ; i < 2 ; ++i ) {
						assert( data.shape()[i] );
						assert( !(data.shape()[i] & (data.shape()[i]-1)) ); //TODO:  Replace pow2 asserts w/ extension-available-or-throw
						assert( data.shape()[i] <= 64 ); //TODO:  Replace check w/ extension-valid-or-throw
					}

					glGenTextures( 1 , &id );
					glBindTexture( GL_TEXTURE_2D , id );
					glTexImage2D( GL_TEXTURE_2D, 0, 3, static_cast<GLsizei>(data.shape()[0]), static_cast<GLsizei>(data.shape()[1]), 0, T::format_enum, T::component_type_enum, data.data() );
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				}
				~texture() {
					glDeleteTextures( 1 , &id );
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
