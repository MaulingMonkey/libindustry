// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 17, 2006 - key_wrapper -> detail::key_data
// Jan 10, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_KEY_DATA
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_KEY_DATA

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/import.hpp>
#include <boost/shared_ptr.hpp>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				namespace detail {
					class key_data {
						HKEY key;
						bool cleanup;
					public:
						operator HKEY() const { return key; }

						key_data(): key(), cleanup(false) {}
						key_data( HKEY key, bool cleanup ): key( key ), cleanup(cleanup) {}
						~key_data() { if ( cleanup ) ::RegCloseKey( key ); }

						bool operator!() const { return key == NULL; }

						friend bool operator==( const key_data & lhs , const key_data & rhs ) {
							return lhs.key == rhs.key;
						}
						friend bool operator!=( const key_data & lhs , const key_data & rhs ) {
							return lhs.key != rhs.key;
						}
					};
					typedef boost::shared_ptr< key_data > key_data_ptr;
				}
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_KEY_DATA
