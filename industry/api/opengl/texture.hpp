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
			template < size_t N > class texture;

			namespace detail {
				template < size_t N > struct texture_dims_to_enum;
				template <> struct texture_dims_to_enum<1> { static const GLenum value = GL_TEXTURE_1D; };
				template <> struct texture_dims_to_enum<2> { static const GLenum value = GL_TEXTURE_2D; };
				//template <> struct texture_dims_to_enum<3> { static const GLenum value = GL_TEXTURE_3D; }

				struct texture_id : boost::noncopyable {
					texture_id()  { glGenTextures   (1,&id); }
					~texture_id() { glDeleteTextures(1,&id); }

					GLuint id;
				};

				template < typename T > struct is_a_texture { enum { value = false }; };
				template < size_t N > struct is_a_texture< texture<N> > { enum { value = true }; };
			}
			
			template <> class texture<2> {
				boost::shared_ptr< detail::texture_id > id;
			public:
				friend inline void glBindTexture( const texture<2>& t ) { ::glBindTexture( GL_TEXTURE_2D, t.id->id ); }

				texture() {}

				template < typename T >
				texture( const boost::multi_array<T,2>& data ) {
					for ( unsigned i = 0 ; i < 2 ; ++i ) {
						assert( data.shape()[i] );
						assert( !(data.shape()[i] & (data.shape()[i]-1)) ); //TODO:  Replace pow2 asserts w/ extension-available-or-throw
						assert( data.shape()[i] <= 64 ); //TODO:  Replace check w/ extension-valid-or-throw
					}

					id.reset( new detail::texture_id );
					glBindTexture( *this );
					glTexImage2D( GL_TEXTURE_2D, 0, 3, static_cast<GLsizei>(data.shape()[0]), static_cast<GLsizei>(data.shape()[1]), 0, T::format_enum, T::component_type_enum, data.data() );
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				}
				~texture() {
				}

				static const GLenum target_enum = GL_TEXTURE_2D;
			};

			typedef texture<1> texture1d;
			typedef texture<2> texture2d;
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
