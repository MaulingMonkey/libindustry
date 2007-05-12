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
		}
	}
}
