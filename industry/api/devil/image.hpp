// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_DEVIL_IMAGE
#define IG_INDUSTRY_API_DEVIL_IMAGE

#include <industry/api/devil/import.hpp>
#include <industry/graphics/image.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <string>
#include <cassert>

namespace industry {
	namespace api {
		namespace devil {
			struct file_not_found   : std::exception { const char * what() const throw() { return "industry::api::devil -- file_not_found"; } };
			struct conversion_error : std::exception { const char * what() const throw() { return "industry::api::devil -- conversion_error"; } };

			namespace detail {
				template < typename T > ILsizei getPixelSizeOf( const T                                       &c ) { return sizeof(T); }
				template < typename T > ILsizei getPixelSizeOf( const graphics::image<T>                      &i ) { return getPixelSizeOf(T()); }
				template < typename T > ILsizei getPixelSizeOf( const boost::shared_ptr< graphics::image<T> > &i ) { return getPixelSizeOf(T()); }

				template < typename T > ILenum getFormatOf( const graphics::rgb<T>                        &c ) { return IL_RGB;  }
				template < typename T > ILenum getFormatOf( const graphics::rgba<T>                       &c ) { return IL_RGBA; }
				template < typename T > ILenum getFormatOf( const graphics::image<T>                      &i ) { return getFormatOf(T()); }
				template < typename T > ILenum getFormatOf( const boost::shared_ptr< graphics::image<T> > &i ) { return getFormatOf(T()); }

				template < template < typename > class C > ILenum getTypeOf( const C<unsigned char>                        &c ) { return IL_BYTE; }
				template < template < typename > class C > ILenum getTypeOf( const C<float>                                &c ) { return IL_FLOAT; }
				template < typename T                    > ILenum getTypeOf( const graphics::image<T>                      &i ) { return getTypeOf(T()); }
				template < typename T                    > ILenum getTypeOf( const boost::shared_ptr< graphics::image<T> > &i ) { return getTypeOf(T()); }

				template < typename T > ILbyte* getDataOf( graphics::image<T>&                            image ) { return (ILbyte*)image.data();  }
				template < typename T > ILbyte* getDataOf( const boost::shared_ptr< graphics::image<T> >& image ) { return (ILbyte*)image->data(); }

				template < typename T > void do_load_init_to_size( graphics::image<T> & image, unsigned width, unsigned height ) {
					image.resize(width,height);
				}
				template < typename T > void do_load_init_to_size( boost::shared_ptr< graphics::image<T> > & image, unsigned width, unsigned height ) {
					if (image) image->resize(width,height);
					else       image = boost::shared_ptr< graphics::image<T> >( new graphics::image<T>(width,height) );
				}

				template < typename T >
				void do_load( T& image, const std::string& filename ) {
					ilEnable( IL_ORIGIN_SET );
					ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
					if (!ilLoadImage( (const ILstring) filename.c_str() )) throw file_not_found();

					unsigned width  = ilGetInteger(IL_IMAGE_WIDTH);
					unsigned height = ilGetInteger(IL_IMAGE_HEIGHT);
					detail::do_load_init_to_size(image,width,height);

					if (!ilConvertImage( detail::getFormatOf(image), detail::getTypeOf(image) )) throw conversion_error();
					assert( detail::getPixelSizeOf(image)==ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL) );
					std::copy( ilGetData(), ilGetData() + width*height*ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL), detail::getDataOf(image) );
				}
			}

			template < typename ColorT >
			boost::shared_ptr< graphics::image< ColorT > > load( const std::string& filename ) {
				boost::shared_ptr< graphics::image< ColorT > > ptr;
				detail::do_load( ptr, filename );
				return ptr;
			}
			template < typename ColorT >
			void load( graphics::image< ColorT >& image, const std::string& filename ) {
				detail::do_load( ptr, filename );
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_DEVIL_IMAGE
