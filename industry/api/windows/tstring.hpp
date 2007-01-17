// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Added to SVN, cleaned up
// Oct 28, 2006 - industry.tstring => industry.windows.tstring
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_TSTRING
#define IG_INDUSTRY_API_WINDOWS_TSTRING

#include <industry/api/windows/config.hpp>
#include <string>

#if !defined( INDUSTRY_OS_WINDOWS )
#error "industry/windows/tstring.hpp : Requires windows access"

#else
#include <tchar.h>
namespace industry {
	namespace api {
		namespace windows {
			typedef std::basic_string< TCHAR > tstring;

#if !defined( UNICODE )
			inline const tstring &      to_tstring( const std::string  & string ) { return string; }
			inline const std::string &  to_string ( const tstring      & string ) { return string; }
#else
			inline tstring              to_tstring( const std::string  & string ) { return tstring( string.begin() , string.end() ); }
			inline const tstring &      to_tstring( const std::wstring & string ) { return string; }

			inline std::string          to_string ( const tstring      & string ) { return std::string( string.begin() , string.end() ); }
			inline const std::wstring & to_wstring( const tstring      & string ) { return string; }
#endif
		}
	}
}

#endif //defined( INDUSTRY_OS_WINDOWS )

#endif //ndef IG_INDUSTRY_API_WINDOWS_TSTRING
