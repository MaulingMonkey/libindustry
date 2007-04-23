// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_API_DEVIL_IMAGE
#define IG_INDUSTRY_API_DEVIL_IMAGE

#include <industry/api/devil/import.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <cassert>

namespace industry {
	namespace api {
		namespace devil {
			struct image_impl : boost::noncopyable {
				ILuint id;
				image_impl()  { id = ilGenImage(); }
				~image_impl() { ilDeleteImage(id); }
			};
			class image {
				boost::shared_ptr< image_impl > impl;
			public:
				friend void ilBindImage( const image& self ) { assert( self.impl ); ::ilBindImage( self.impl->id ); }

				image() {}
				image( const std::string& filename ) {
					impl.reset( new image_impl );
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_DEVIL_IMAGE
