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
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_ERRORS
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_ERRORS

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/import.hpp>
#include <exception>

namespace industry {
	namespace api {
		namespace windows {
			void throw_error( LONG );
			struct error                            : std::exception { const char * what() const { return "Unknown Error"                      ""                                                                              ; } };
			struct access_error                     : error          { const char * what() const { return "Access Error"                       " (attempted to access an unknown resource that you do not have permission to)" ; } };
			struct missing_error                    : error          { const char * what() const { return "Missing Error"                      " (attempted to access an unknown resource that does not exist)"                ; } };

			namespace registry {
				struct error                        : windows::error { protected: ~error() {} };

				void throw_key_error( LONG );
				struct null_key_error               : std::exception { const char * what() const { return "Registry Key Null Error"            " (attempted to access a key reference which did not refer to a key)"           ; } };
				struct missing_key_error            : missing_error  { const char * what() const { return "Registry Key Missing Error"         " (attempted to access a key that does not exist)"                              ; } };
				struct key_access_error             : access_error   { const char * what() const { return "Registry Key Access Error"          " (attempted to access a key that you do not have permission to)"               ; } };
				
				void throw_value_error( LONG );
				struct invalid_value_type_error     : error          { const char * what() const { return "Registry Value Type Error"          " (attempted to access a key-value of an incompatible type)"                    ; } };
				struct malformed_value_error        : error          { const char * what() const { return "Registry Value Malformation Error"  " (attempted to access a key-value containing invalid data)"                    ; } };
				struct missing_value_error          : missing_error  { const char * what() const { return "Registry Value Missing Error"       " (attempted to access a key-value that does not exist)"                        ; } };
				struct value_access_error           : access_error   { const char * what() const { return "Registry Value Access Error"        " (attempted to access a key-value that you do not have permission to)"         ; } };
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_ERRORS
