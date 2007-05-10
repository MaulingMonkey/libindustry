// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_FREETYPE_LOCATOR
#define IG_INDUSTRY_API_FREETYPE_LOCATOR

#include <industry/api/freetype/import.hpp>
#include <industry/api/freetype/library.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace freetype {
			struct face_info {
				std::string filename;
				unsigned index;
				unsigned size;
			};
			class locator {
				FT_Library library_;
			protected:
				FT_Library library() const { return library_; }
			public:
				locator( FT_Library library ): library_(library) {} // NOTE: Unmanaged copy
				locator( const freetype::library& library ): library_(library.handle()) {}

				virtual face_info find_face_info( const std::string& name );
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_LOCATOR
