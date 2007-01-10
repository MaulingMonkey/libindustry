// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 28, 2006 - Created

#include <industry/config.hpp>

#if defined( INDUSTRY_OS_WINDOWS )
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
			void throw_registry_error( LONG error ) {
				switch( error ) {
					case ERROR_SUCCESS:        return;
					case ERROR_ACCESS_DENIED:  throw registry_access_error();
					case ERROR_FILE_NOT_FOUND: throw missing_registry_error();
					default:                   throw_error( error );
				}
			}
			void throw_registry_key_error( LONG error ) {
				switch ( error ) {
					case ERROR_SUCCESS:        return;
					case ERROR_ACCESS_DENIED:  throw registry_key_access_error();
					case ERROR_FILE_NOT_FOUND: throw missing_registry_key();
					default:                   throw_registry_error( error );
				}
			}
			void throw_registry_key_value_error( LONG error ) {
				switch ( error ) {
					case ERROR_SUCCESS:        return;
					case ERROR_ACCESS_DENIED:  throw registry_key_value_access_error();
					case ERROR_FILE_NOT_FOUND: throw missing_registry_key_value();
					default:                   throw_registry_error( error );
				}
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
