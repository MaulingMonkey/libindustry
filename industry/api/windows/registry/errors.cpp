// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 15, 2007 - Replaced diamond exception trees
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 28, 2006 - Created

#include <industry/config.hpp>

#if !defined( INDUSTRY_OS_WINDOWS )
//Nothing

#elif defined( _MSC_VER ) && !defined( _MSC_EXTENSIONS )
#include <boost/preprocessor/stringize.hpp>
#pragma message( __FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ") : warning: industry/api/windows/* will not be compiled without language extensions (windows.h pukes)" )

#else
#include <industry/api/windows/registry/errors.hpp>

namespace industry {
	namespace api {
		namespace windows {
			void throw_error( LONG error ) {
				switch( error ) {
					case ERROR_SUCCESS:        return;
					case ERROR_ACCESS_DENIED:  throw access_error();
					case ERROR_FILE_NOT_FOUND: throw missing_error();
					default:                   throw windows::error();
				}
			}
			namespace registry {
				void throw_key_error( LONG error ) {
					switch ( error ) {
						case ERROR_SUCCESS:        return;
						case ERROR_ACCESS_DENIED:  throw key_access_error();
						case ERROR_FILE_NOT_FOUND: throw missing_key_error();
						default:                   throw_error( error );
					}
				}
				void throw_value_error( LONG error ) {
					switch ( error ) {
						case ERROR_SUCCESS:        return;
						case ERROR_ACCESS_DENIED:  throw value_access_error();
						case ERROR_FILE_NOT_FOUND: throw missing_value_error();
						default:                   throw_error( error );
					}
				}
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
