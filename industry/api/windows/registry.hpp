// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 15, 2007 - Eliminiated INDUSTRY_UNICODE_* in favor of the plain UNICODE define.
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 26, 2006 - "win32" replaced with "windows" - should be mostly architecture independant
// Aug 17, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY
#define IG_INDUSTRY_API_WINDOWS_REGISTRY

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/registry/key.hpp>
#include <industry/api/windows/import.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				class registry_reference {
				public:
					registry_reference();

					key operator[]( const std::string & name ) const;
					key operator/ ( const std::string & name ) const;
				private:
					key lookup_root_key( const std::string & name ) const;
#if defined( UNICODE )
				public:
					key operator[]( const std::wstring & name ) const;
					key operator/ ( const std::wstring & name ) const;
				private:
					key lookup_root_key( const std::wstring & name ) const;
#endif
				public:
					const key
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
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY
