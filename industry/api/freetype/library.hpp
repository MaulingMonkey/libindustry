// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_FREETYPE_LIBRARY
#define IG_INDUSTRY_API_FREETYPE_LIBRARY

#include <industry/api/freetype/import.hpp>

namespace industry {
	namespace api {
		namespace freetype {
			class library {
				FT_Library data;
			public:
				library()  { FT_Init_FreeType( &data ); }
				~library() { FT_Done_FreeType(  data ); }
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_LIBRARY
