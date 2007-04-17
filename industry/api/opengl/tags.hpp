// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TAGS
#define IG_INDUSTRY_API_OPENGL_TAGS

namespace industry {
	namespace api {
		namespace opengl {
			struct minimal     {}; // Textures: Lowest common denominator (e.g. 64x64 max)
			struct non_pow2    {}; // Textures: Supports non pow-2 textures for everything
			struct rectangular {}; // Textures: Supports minimal non pow-2 textures
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TAGS
