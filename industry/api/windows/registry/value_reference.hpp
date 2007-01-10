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

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_REFERENCE
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_REFERENCE

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/registry/value_wrapper.hpp>
#include <industry/api/windows/tstring.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			struct QWORD { char data[8]; };
			inline bool operator==( const QWORD & lhs , const QWORD & rhs ) { return  std::equal( lhs.data , lhs.data + 8 , rhs.data ); }
			inline bool operator!=( const QWORD & lhs , const QWORD & rhs ) { return !std::equal( lhs.data , lhs.data + 8 , rhs.data ); }

			class registry_value_reference {
				registry_value_wrapper value;
			public:
				registry_value_reference( const registry_value_reference & other ) : value( other.value ) {}
				registry_value_reference( const registry_value_wrapper   & value ) : value( value ) {}
				~registry_value_reference() {}

				std::vector< char >          to_data     () const;
				DWORD                        to_dword    () const;
				QWORD                        to_qword    () const;

				std::string                  to_string   () const;
#if defined( INDUSTRY_UNICODE_ENABLED )
				std::wstring                 to_wstring  () const;
				tstring                      to_tstring  () const;
#endif

				std::vector< std::string  >  to_strings  () const;
#if defined( INDUSTRY_UNICODE_ENABLED )
				std::vector< std::wstring >  to_wstrings () const;
				std::vector< tstring      >  to_tstrings () const;
#endif
			};

			bool operator==( const registry_value_reference & value , const registry_value_reference & registry_value );
			//NOTE:  The above currently no implementation - just to have link error instead of infinite recursion bellow.

			template < typename T >
			bool operator==( const T & value , const registry_value_reference & registry_value ) {
				//NOTE:  This simply porks the work onto the operator==s with the registry_value as their LHS argument.
				return registry_value == value;
			}

			inline bool operator==( const registry_value_reference & reference , DWORD data ) {
				return reference.to_dword() == data;
			}
			inline bool operator==( const registry_value_reference & reference , QWORD data ) {
				return reference.to_qword() == data;
			}
			inline bool operator==( const registry_value_reference & reference , const std::vector< char > & data ) {
				return reference.to_data() == data;
			}
			inline bool operator==( const registry_value_reference & reference , const std::string & data ) {
				return reference.to_string() == data;
			}
#if defined( INDUSTRY_UNICODE_ENABLED )
			inline bool operator==( const registry_value_reference & reference , const std::wstring & data ) {
				return reference.to_wstring() == data;
			}
#endif

			inline std::ostream  & operator<<( std::ostream  & os  , const registry_value_reference & registry_value ) { return os  << registry_value.to_string();  }
#if defined( INDUSTRY_UNICODE_ENABLED )
			inline std::wostream & operator<<( std::wostream & wos , const registry_value_reference & registry_value ) { return wos << registry_value.to_wstring(); }
#endif
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_REFERENCE
