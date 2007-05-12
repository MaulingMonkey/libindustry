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
#include <boost/shared_ptr.hpp>

namespace industry {
	namespace api {
		namespace freetype {
			namespace detail {
				struct library_data {
					FT_Library handle;

					library_data()  { FT_Init_FreeType( &handle ); }
					~library_data() { FT_Done_FreeType(  handle ); }
				};
			}
			class library {
				friend class face;
				boost::shared_ptr< detail::library_data > impl;
			public:
				FT_Library handle() const { return impl->handle; }
				library() {
					impl.reset( new detail::library_data );
				}
				~library() {
				}
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_FREETYPE_LIBRARY
