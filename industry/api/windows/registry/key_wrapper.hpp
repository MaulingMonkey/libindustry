// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_WRAPPER
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_WRAPPER

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/import.hpp>

namespace industry {
	namespace windows {
		class registry_key_wrapper {
		public:
			HKEY key;
			bool cleanup;

			registry_key_wrapper() : key() , cleanup( false ) {}
			registry_key_wrapper( HKEY key , bool cleanup ) : key( key ) , cleanup( cleanup ) {}
			~registry_key_wrapper() { if ( cleanup ) ::RegCloseKey( key ); }
		};
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_WRAPPER
