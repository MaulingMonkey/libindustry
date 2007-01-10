// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
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
			struct error                            : std::exception                                    { const char * what() const { return "Unknown Error"                          ""                                                                              ; } };
			struct access_error                     : error                                             { const char * what() const { return "Access Error"                           " (attempted to access an unknown resource that you do not have permission to)" ; } };
			struct missing_error                    : error                                             { const char * what() const { return "Missing Error"                          " (attempted to access an unknown resource that does not exist)"                ; } };

			struct registry_error                   : error                                             { const char * what() const { return "Registry Error"                         " (an unknown registry error has occured)"                                      ; } };
			struct registry_access_error            : registry_error , access_error                     { const char * what() const { return "Registry Access Error"                  " (attempted to access a key or key-value that you do not have permission to)"  ; } };
			struct missing_registry_error           : registry_error , missing_error                    { const char * what() const { return "Registry Missing Error"                 " (attempted to access a key or key-value that does not exist)"                 ; } };

			struct registry_key_error               : error                                             { const char * what() const { return "Registry Key Error"                     " (attempted to access a key, unknown failure cause)"                           ; } };
			struct null_registry_key_error          : registry_key_error                                { const char * what() const { return "Registry Key Null Error"                " (attempted to access a key reference which did not refer to a key)"           ; } };
			struct missing_registry_key             : registry_key_error , missing_registry_error       { const char * what() const { return "Registry Key Missing Error"             " (attempted to access a key that does not exist)"                              ; } };
			struct registry_key_access_error        : registry_key_error , registry_access_error        { const char * what() const { return "Registry Key Access Error"              " (attempted to access a key that you do not have permission to)"               ; } };
			
			struct registry_key_value_error         : error                                             { const char * what() const { return "Registry Key Value Error"               " (attempted to access a key-value, unknown failure cause)"                     ; } };
			struct invalid_registry_key_value_type  : registry_key_value_error                          { const char * what() const { return "Registry Key Value Type Error"          " (attempted to access a key-value of an incompatible type)"                    ; } };
			struct malformed_registry_key_value     : registry_key_value_error                          { const char * what() const { return "Registry Key Value Malformation Error"  " (attempted to access a key-value containing invalid data)"                    ; } };
			struct missing_registry_key_value       : registry_key_value_error , missing_registry_error { const char * what() const { return "Registry Key Value Missing Error"       " (attempted to access a key-value that does not exist)"                        ; } };
			struct registry_key_value_access_error  : registry_key_value_error , registry_access_error  { const char * what() const { return "Registry Key Value Access Error"        " (attempted to access a key-value that you do not have permission to)"         ; } };

			void throw_error( LONG );
			void throw_registry_error( LONG );
			void throw_registry_key_error( LONG );
			void throw_registry_key_value_error( LONG );
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_ERRORS
