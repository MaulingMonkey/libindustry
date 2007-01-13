// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 13, 2006 - Created
//
// FIXME:  Error handling, you stupid git!

#include <industry/config.hpp>

#if defined( INDUSTRY_OS_WINDOWS )
#include <industry/api/windows/registry/value_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <limits>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			registry_value_iterator& registry_value_iterator::operator++( /* prefix  */     ) {
				LONG error        = ERROR_MORE_DATA;
				DWORD buffer_size = 15; //arbitrary, suggest a power of 2 minus 1

				while ( error == ERROR_MORE_DATA ) {
					buffer_size += 1; //when [out], dosn't include the terminating _T('\0') - hoever [in] expects it, compensate for this Win32 API idiotcyncracy.
					buffer.resize( buffer_size );
					if ( buffer.capacity() > buffer.size() ) buffer.resize( buffer.capacity() );
					assert( buffer.size() >= buffer_size );
					assert( buffer.size() <= std::numeric_limits< DWORD >::max() );
					buffer_size = DWORD( buffer.size() );

					error = RegEnumValue( /* hKey                  */ key->key
										, /* dwIndex               */ index
										, /* lpValueName           */ buffer.empty() ? NULL : &buffer[0]
										, /* lpcValueName [in/out] */ &buffer_size
										, /* lpReserved            */ NULL
										, /* lpType                */ NULL
										, /* lpData                */ NULL
										, /* lpcbData              */ NULL
										);
					if ( buffer.size() == 0 && buffer_size != 0 && error == ERROR_SUCCESS ) error = ERROR_MORE_DATA;
				}
				if ( error == ERROR_SUCCESS ) buffer.resize( buffer_size );
				//else ERROR HANDLING HERE
				return *this;
			}
			registry_value_iterator  registry_value_iterator::operator++( /* postfix */ int ) {
				registry_value_iterator copy(*this);
				++*this;
				return copy;
			}
		}
	}
}

#endif //defined( INDUSTRY_OS_WINDOWS )