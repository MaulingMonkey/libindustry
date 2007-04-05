// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_OPENGL_TEXTURE
#define IG_INDUSTRY_API_OPENGL_TEXTURE

#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/color.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4267 )
#endif
#include <boost/multi_array.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace industry {
	namespace api {
		namespace opengl {
			template < size_t N >
			class texture {
			public:
				template < typename T >
				texture( const boost::multi_array< T , N >& ) {
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_OPENGL_TEXTURE
