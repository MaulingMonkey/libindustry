// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 13, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR

#include <industry/api/windows/import.hpp>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			class registry_value_iterator {
			public:
				registry_value_iterator( HKEY key ); //start iterator (at beginning of list of values contained by the key)
				registry_value_iterator();           //end iterator
			private:
				DWORD index;
				std::vector< wchar_t > buffer;
			};
#if 0 //-----------------------------------------------------------------------------------------
				struct RegEnumValueCaller {
					HKEY key;
					std::vector< wchar_t > name_buffer;

					RegEnumValueCaller( HKEY key ) : key( key ) {}

					LONG operator()( DWORD index ) {
						LONG  error = ERROR_MORE_DATA;
						DWORD name_buffer_size = 16;

						while ( error == ERROR_MORE_DATA ) {
							name_buffer_size += 1; //when [out], dosn't include the terminating _T('\0') - hoever [in] expects it, compensate for this Win32 API idiotcyncracy.
							name_buffer.resize( name_buffer_size );
							if ( name_buffer.capacity() > name_buffer.size() ) name_buffer.resize( name_buffer.capacity() );
							assert( name_buffer.size() >= name_buffer_size );
							assert( name_buffer.size() <= std::numeric_limits< DWORD >::max() );
							name_buffer_size = DWORD( name_buffer.size() );

							error = RegEnumValue( /* hKey                  */ key
												, /* dwIndex               */ index
												, /* lpValueName           */ name_buffer.empty() ? NULL : &name_buffer[0]
												, /* lpcValueName [in/out] */ &name_buffer_size
												, /* lpReserved            */ NULL
												, /* lpType                */ NULL
												, /* lpData                */ NULL
												, /* lpcbData              */ NULL
												);
							if ( name_buffer.size() == 0 && name_buffer_size != 0 && error == ERROR_SUCCESS ) error = ERROR_MORE_DATA;
						}
						if ( error == ERROR_SUCCESS ) name_buffer.resize( name_buffer_size );
						return error;
					}
				};
#endif //-----------------------------------------------------------------------------------------
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR
