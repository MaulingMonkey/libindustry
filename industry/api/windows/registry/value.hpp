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

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/detail/value_data.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/tstring.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				struct QWORD { char data[8]; };
				inline bool operator==( const QWORD & lhs , const QWORD & rhs ) { return  std::equal( lhs.data , lhs.data + 8 , rhs.data ); }
				inline bool operator!=( const QWORD & lhs , const QWORD & rhs ) { return !std::equal( lhs.data , lhs.data + 8 , rhs.data ); }

				class value {
					detail::value_data_ptr impl;
				public:
					value( const value                    & other      ): impl( other.impl ) {}
					value( const detail::value_data_ptr   & value_data ): impl( value_data ) {}
					value( const detail::key_data_ptr     & key_data   , const tstring & name ): impl( new detail::value_data( key_data , name ) ) {}
					~value() {}

					std::vector< char >          to_data     () const;
					DWORD                        to_dword    () const;
					QWORD                        to_qword    () const;

					std::string                  to_string   () const;
					tstring                      to_tstring  () const;
#if defined( UNICODE )
					std::wstring                 to_wstring  () const;
#endif

					std::vector< std::string  >  to_strings  () const;
					std::vector< tstring      >  to_tstrings () const;
#if defined( UNICODE )
					std::vector< std::wstring >  to_wstrings () const;
#endif
				};

				bool operator==( const value & , const value & );
				//NOTE:  The above currently no implementation - just to have link error instead of infinite recursion bellow.

				template < typename T >
				bool operator==( const T & data , const value & v ) {
					//NOTE:  This simply porks the work onto the operator==s with the registry_value as their LHS argument.
					return data == comparee;
				}

				inline bool operator==( const value & v , DWORD data ) {
					return v.to_dword() == data;
				}
				inline bool operator==( const value & v , QWORD data ) {
					return v.to_qword() == data;
				}
				inline bool operator==( const value & v , const std::vector< char > & data ) {
					return v.to_data() == data;
				}
				inline bool operator==( const value & v , const std::string & data ) {
					return v.to_tstring() == to_tstring(data);
				}
#if defined( UNICODE )
				inline bool operator==( const value & v , const std::wstring & data ) {
					return v.to_wstring() == data;
				}
#endif

				inline std::ostream  & operator<<( std::ostream  & os  , const value & v ) { return os  << v.to_string();  }
#if defined( UNICODE )
				inline std::wostream & operator<<( std::wostream & wos , const value & v ) { return wos << v.to_wstring(); }
#endif
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE
