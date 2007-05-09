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
			namespace { freetype::locator default_locator; }

			face::face( const boost::shared_ptr< detail::library_data >& library, const std::string& name ) {
				face_info info = default_locator.find_face_info(name);
				//impl.reset( new detail::face_data( library , font.to_string().c_str() , 0 /*????*/ ) );
			}
		}
	}
}
