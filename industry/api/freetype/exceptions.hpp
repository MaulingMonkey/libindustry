// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_FREETYPE_EXCEPTIONS
#define IG_INDUSTRY_API_FREETYPE_EXCEPTIONS

#include <boost/shared_ptr.hpp>
#include <exception>

namespace industry {
	namespace api {
		namespace freetype {
			struct font_not_found           : std::exception { const char * what() const { return "industry::api::freetype -- font not found";           } };
			struct invalid_font_description : std::exception { const char * what() const { return "industry::api::freetype -- invalid font description"; } };
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_EXCEPTIONS
