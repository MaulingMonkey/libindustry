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
						FT_New_Face( library->handle, filename, index, &handle );
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
				~face() {}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_FACE
