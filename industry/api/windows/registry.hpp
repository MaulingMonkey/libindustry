// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 26, 2006 - "win32" replaced with "windows" - should be mostly architecture independant
// Aug 17, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY
#define IG_INDUSTRY_API_WINDOWS_REGISTRY

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/registry/key_reference.hpp>
#include <industry/api/windows/registry/key_wrapper.hpp>
#include <industry/api/windows/registry/value_reference.hpp>
#include <industry/api/windows/registry/value_wrapper.hpp>
#include <industry/api/windows/import.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace windows {
			class registry_reference {
			public:
				registry_reference();

				registry_key_reference operator[]( const std::string & name ) const;
				registry_key_reference operator/ ( const std::string & name ) const;
			private:
				registry_key_reference lookup_root_key( const std::string & name ) const;
			public:
#if defined( INDUSTRY_UNICODE_ENABLED )
				registry_key_reference operator[]( const std::wstring & name ) const;
				registry_key_reference operator/ ( const std::wstring & name ) const;
			private:
				registry_key_reference lookup_root_key( const std::wstring & name ) const;
			public:
#endif

				const registry_key_reference
					hkey_classes_root   ,
					hkey_current_user   ,
					hkey_local_machine  ,
					hkey_users          ,
					hkey_current_config ;
			};

			extern const registry_reference registry;
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY
