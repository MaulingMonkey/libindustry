// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_FREETYPE_FACE
#define IG_INDUSTRY_API_FREETYPE_FACE

#include <industry/api/freetype/import.hpp>
#include <industry/api/freetype/library.hpp>
#include <industry/api/freetype/locator.hpp>
#include <industry/math/vector.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace freetype {
			namespace detail {
				struct face_data {
					boost::shared_ptr< detail::library_data > library;
					FT_Face handle;

					face_data( const boost::shared_ptr< detail::library_data >& library
					         , const char* filename
					         , unsigned index
					         ): library(library)
					{
						if (FT_New_Face( library->handle, filename, index, &handle )) throw std::runtime_error( "Error opening face" );
					}
					~face_data() {
						FT_Done_Face( handle );
					}
				};
			}

			class face {
				boost::shared_ptr< detail::face_data > impl;
			public:
				face() {}
				face( const boost::shared_ptr< detail::library_data >& library, const std::string& name );
				face( const freetype::library& library, const std::string& name );
				~face() {}

				FT_Glyph_Metrics char_metrics( FT_ULong charcode ) const;
				FT_Vector char_kerning( FT_ULong left, FT_ULong right ) const;
				void char_blit( FT_ULong c, math::vector<int,2> offset, boost::multi_array< FT_Byte, 2 > & target ) const;
			private:
				void initialize( const boost::shared_ptr< detail::library_data >& library, const std::string& name );
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_FACE
