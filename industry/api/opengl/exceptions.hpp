// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_EXCEPTIONS
#define IG_INDUSTRY_API_OPENGL_EXCEPTIONS

#include <exception>

namespace industry {
	namespace api {
		namespace opengl {
			struct texture_size_unavailable  : std::exception { const char * what() const { return "industry::api::opengl -- texture_size_unavailable" ; } };
			struct texture_shape_unavailable : std::exception { const char * what() const { return "industry::api::opengl -- texture_shape_unavailable"; } };
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_EXCEPTIONS
