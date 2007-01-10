// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_REFERENCE
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_REFERENCE

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/key_wrapper.hpp>
#include <industry/api/windows/registry/value_reference.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			class registry_key_reference {
				boost::shared_ptr< registry_key_wrapper > key;
			public:
				registry_key_reference( HKEY );
				~registry_key_reference();

				registry_key_reference( const registry_key_reference & parent , const std::string & name );
				registry_value_reference operator[]( const std::string & name ) const;
				registry_value_reference operator% ( const std::string & name ) const;
				registry_key_reference   operator/ ( const std::string & name ) const;
				registry_key_reference & operator/=( const std::string & name ) { return *this = *this / name; }
				friend std::vector< registry_key_reference   > find         ( const registry_key_reference & , const std::string & key_name   );
				friend std::vector< registry_value_reference > find_value   ( const registry_key_reference & , const std::string & value_name );
				friend std::vector< registry_key_reference   > find_if      ( const registry_key_reference & , const boost::function< bool ( const char* , const char* ) > & key_name   );
				friend std::vector< registry_value_reference > find_value_if( const registry_key_reference & , const boost::function< bool ( const char* , const char* ) > & value_name );

#if defined( INDUSTRY_UNICODE_ENABLED )
				registry_key_reference( const registry_key_reference & parent , const std::wstring & name );
				registry_value_reference operator[]( const std::wstring & name ) const;
				registry_value_reference operator% ( const std::wstring & name ) const;
				registry_key_reference   operator/ ( const std::wstring & name ) const;
				registry_key_reference & operator/=( const std::wstring & name ) { return *this = *this / name; }
				friend std::vector< registry_key_reference   > find         ( const registry_key_reference & , const std::wstring & key_name   );
				friend std::vector< registry_value_reference > find_value   ( const registry_key_reference & , const std::wstring & value_name );
				friend std::vector< registry_key_reference   > find_if      ( const registry_key_reference & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & key_name   );
				friend std::vector< registry_value_reference > find_value_if( const registry_key_reference & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & value_name );
#endif
			};
			template < typename T > bool operator==( const registry_key_reference & registry_key , const T & value ) { return registry_key % "" == value; }
			template < typename T > bool operator==( const T & value , const registry_key_reference & registry_key ) { return registry_key % "" == value; }
			
			inline std::ostream & operator<<( std::ostream & os , const registry_key_reference & registry_key ) { return os << (registry_key % ""); }
#if defined( INDUSTRY_UNICODE_ENABLED )
			inline std::wostream & operator<<( std::wostream & wos , const registry_key_reference & registry_key ) { return wos << (registry_key % ""); }
#endif
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY_REFERENCE
