// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 26, 2006 - Created

#include <industry/config.hpp>

#if !defined( INDUSTRY_OS_WINDOWS )
//Nothing

#elif defined( _MSC_VER ) && !defined( _MSC_EXTENSIONS )
#include <boost/preprocessor/stringize.hpp>
#pragma message( __FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ") : warning: industry/api/windows/* will not be compiled without language extensions (windows.h pukes)" )

#else
#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/registry.hpp>
#include <industry/api/windows/import.hpp>
#include <stdexcept>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				const registry_reference registry;
				
				key registry_reference::operator[]( const std::string & name ) const {
					std::string::size_type delim = name.find( "\\" );
					if ( delim == std::string::npos ) {
						return lookup_root_key( name );
					} else {
						key k = lookup_root_key( std::string( name.begin() , name.begin() + delim ) );
						if ( delim != name.size()-1 ) k /= std::string( name.begin() + delim + 1 , name.end() );
						return k;
					}
				}
				key registry_reference::operator/ ( const std::string & name ) const {
					return (*this)[ name ];
				}
				key registry_reference::lookup_root_key( const std::string & name ) const {
					if ( name == "HKEY_CLASSES_ROOT"   ) return hkey_classes_root;
					if ( name == "HKEY_CURRENT_USER"   ) return hkey_current_user;
					if ( name == "HKEY_LOCAL_MACHINE"  ) return hkey_local_machine;
					if ( name == "HKEY_USERS"          ) return hkey_users;
					if ( name == "HKEY_CURRENT_CONFIG" ) return hkey_current_config;
					throw missing_key_error();
				}
#if defined( UNICODE )
				key registry_reference::operator[]( const std::wstring & name ) const {
					std::wstring::size_type delim = name.find( L"\\" );
					if ( delim == std::wstring::npos ) {
						return lookup_root_key( name );
					} else {
						key k = lookup_root_key( std::wstring( name.begin() , name.begin() + delim ) );
						if ( delim != name.size()-1 ) k /= std::wstring( name.begin() + delim + 1 , name.end() );
						return k;
					}
				}
				key registry_reference::operator/ ( const std::wstring & name ) const {
					return (*this)[ name ];
				}
				key registry_reference::lookup_root_key( const std::wstring & name ) const {
					if ( name == L"HKEY_CLASSES_ROOT"   ) return hkey_classes_root;
					if ( name == L"HKEY_CURRENT_USER"   ) return hkey_current_user;
					if ( name == L"HKEY_LOCAL_MACHINE"  ) return hkey_local_machine;
					if ( name == L"HKEY_USERS"          ) return hkey_users;
					if ( name == L"HKEY_CURRENT_CONFIG" ) return hkey_current_config;
					throw missing_key_error();
				}
#endif
				registry_reference::registry_reference()
					: hkey_classes_root   ( HKEY_CLASSES_ROOT   )
					, hkey_current_user   ( HKEY_CURRENT_USER   )
					, hkey_local_machine  ( HKEY_LOCAL_MACHINE  )
					, hkey_users          ( HKEY_USERS          )
					, hkey_current_config ( HKEY_CURRENT_CONFIG )
				{
				}
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
