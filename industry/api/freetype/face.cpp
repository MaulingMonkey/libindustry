// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/config.hpp>
#include <industry/api/freetype/face.hpp>

namespace industry {
	namespace api {
		namespace freetype {
			face::face( const boost::shared_ptr< detail::library_data >& library, const std::string& name ) {
				initialize( library, name );
			}
			face::face( const freetype::library& library, const std::string& name ) {
				initialize( library.impl, name );
			}
			void face::initialize( const boost::shared_ptr< detail::library_data >& library, const std::string& name ) {
				freetype::locator locator(library->handle);
				face_info info = locator.find_face_info(name);
				impl.reset( new detail::face_data( library, info.filename.c_str(), info.index ) );
				FT_Set_Pixel_Sizes( impl->handle , info.size , 0 );
			}
			FT_Glyph_Metrics face::char_metrics( FT_ULong charcode ) const {
				assert( FT_Load_Char( impl->handle, charcode, FT_LOAD_DEFAULT ) );
				return impl->handle->glyph->metrics;
			}
			FT_Vector face::char_kerning( FT_ULong left, FT_ULong right ) const {
				FT_UInt left_glyph  = FT_Get_Char_Index( impl->handle, left  );
				FT_UInt right_glyph = FT_Get_Char_Index( impl->handle, right );
				FT_Vector kern;
				assert( FT_Get_Kerning( impl->handle, left_glyph, right_glyph, FT_KERNING_DEFAULT, &kern ) );
				return kern;
			}
			void face::char_blit( FT_ULong charcode, math::vector<int,2> offset, boost::multi_array< FT_Byte, 2 > & target ) const {
				offset.x += impl->handle->glyph->bitmap_left;
				offset.y += impl->handle->glyph->bitmap_top;
				FT_Load_Char( impl->handle, charcode, FT_LOAD_RENDER );
				const FT_Bitmap & bitmap = impl->handle->glyph->bitmap;
				assert( bitmap.pixel_mode == FT_PIXEL_MODE_GRAY );
				assert( 0 <= offset.x && offset.x < target.shape()[0] );
				assert( 0 <= offset.y && offset.y < target.shape()[1] );

				for ( int y = 0 ; y < bitmap.rows ; ++y ) {
					for ( int x = 0 ; x < bitmap.width ; ++x ) {
						target[ x+offset.x ][ y+offset.y ] = *(bitmap.buffer + x + y * bitmap.pitch);
					}
				}
			}
		}
	}
}
