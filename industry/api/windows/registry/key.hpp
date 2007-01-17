// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 17, 2007 - registry_key_reference -> registry::key, centralized logic around tstring
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/detail/key_data.hpp>
#include <industry/api/windows/registry/value.hpp>
#include <industry/api/windows/registry/value_iterator.hpp>
#include <industry/api/windows/tstring.hpp>
#include <industry/range.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				class key {
					detail::key_data_ptr impl;

					void  initialize  ( const key & parent , const tstring & name );
					key   select_key  ( const tstring & name ) const;
					value select_value( const tstring & name ) const;
				public:
					key();
					key( HKEY );
					~key();

					template < typename CharT > key( const key & parent , const std::basic_string< CharT > & name ) { initialize(parent,to_tstring(name)); }
					template < typename CharT > value operator[]( const std::basic_string< CharT > & name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > value operator% ( const std::basic_string< CharT > & name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > key   operator/ ( const std::basic_string< CharT > & name ) const { return select_key  (to_tstring(name)); }
					template < typename CharT > key & operator/=( const std::basic_string< CharT > & name ) { return *this = *this / name; }

					//friend industry::range< key_iterator   > each_key   ( const key & );
					friend industry::range< value_iterator > each_value ( const key & );

#if 0				//Deprecated:
					friend std::vector< key   > find_         ( const key & , const std::string & key_name   );
					friend std::vector< value > find_value_   ( const key & , const std::string & value_name );
					friend std::vector< key   > find_if_      ( const key & , const boost::function< bool ( const char* , const char* ) > & key_name   );
					friend std::vector< value > find_value_if_( const key & , const boost::function< bool ( const char* , const char* ) > & value_name );

#if defined( UNICODE )
					//Depreciated:
					friend std::vector< key   > find_         ( const key & , const std::wstring & key_name   );
					friend std::vector< value > find_value_   ( const key & , const std::wstring & value_name );
					friend std::vector< key   > find_if_      ( const key & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & key_name   );
					friend std::vector< value > find_value_if_( const key & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & value_name );
#endif
#endif
				};
				template < typename T > bool operator==( const key & k , const T & value ) { return k % "" == value; }
				template < typename T > bool operator==( const T & value , const key & k ) { return k % "" == value; }
				template < typename T > bool operator!=( const key & k , const T & value ) { return !(k == value); }
				template < typename T > bool operator!=( const T & value , const key & k ) { return !(k == value); }

				template < typename CharT >
				inline std::basic_ostream< CharT > & operator<<( std::basic_ostream< CharT > & os , const key & k ) { return os << (k % ""); }
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY
