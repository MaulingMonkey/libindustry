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
				template < typename T > ILenum getFormatOf( const graphics::rgb<T> & ) { return IL_RGB;  }
				template < typename T > ILenum getFormatOf( const graphics::rgba<T>& ) { return IL_RGBA; }
				template < template < typename > class C > ILenum getTypeOf( const C<unsigned char>& ) { return IL_BYTE; }
				template < template < typename > class C > ILenum getTypeOf( const C<float>        & ) { return IL_FLOAT; }
			}

			template < typename ColorT >
			boost::shared_ptr< graphics::image< ColorT > > load( const std::string& filename ) {
				ilEnable( IL_ORIGIN_SET );
				ilOriginFunc( IL_ORIGIN_UPPER_LEFT );
				if (!ilLoadImage( (const ILstring) filename.c_str() )) throw file_not_found();
				
				unsigned width  = ilGetInteger(IL_IMAGE_WIDTH);
				unsigned height = ilGetInteger(IL_IMAGE_HEIGHT);
				boost::shared_ptr< graphics::image< ColorT > > image(  new graphics::image<ColorT>(width,height)  );

				if (!ilConvertImage( detail::getFormatOf(ColorT()), detail::getTypeOf(ColorT()) )) throw conversion_error();
				assert( sizeof(ColorT)==ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL) );
				std::copy( ilGetData(), ilGetData() + width*height*ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL), (ILbyte*)image->data() );

				return image;
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_DEVIL_IMAGE
