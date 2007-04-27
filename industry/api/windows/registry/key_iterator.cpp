// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// FIXME:  Error handling, you stupid git!

#include <industry/config.hpp>

#if !defined( INDUSTRY_OS_WINDOWS )
//Nothing

#elif defined( _MSC_VER ) && !defined( _MSC_EXTENSIONS )
#include <boost/preprocessor/stringize.hpp>
#pragma message( __FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ") : warning: industry/api/windows/* will not be compiled without language extensions (windows.h pukes)" )

#else
#include <industry/api/windows/registry/key_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <limits>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				key_iterator& key_iterator::operator++( /* prefix  */     ) {
					LONG error        = ERROR_MORE_DATA;
					DWORD buffer_size = 15; //arbitrary, suggest a power of 2 minus 1

					while ( error == ERROR_MORE_DATA ) {
						buffer_size += 1; //when [out], dosn't include the terminating _T('\0') - hoever [in] expects it, compensate for this Win32 API idiotcyncracy.
						buffer.resize( buffer_size );
						if ( buffer.capacity() > buffer.size() ) buffer.resize( buffer.capacity() );
						assert( buffer.size() >= buffer_size );
						assert( buffer.size() <= std::numeric_limits< DWORD >::max() );
						buffer_size = DWORD( buffer.size() );

						error = RegEnumKeyEx( /* hKey                  */ *key
						                    , /* dwIndex               */ index
						                    , /* lpName                */ buffer.empty() ? NULL : &buffer[0]
						                    , /* lpcchName [in/out]    */ &buffer_size
						                    , /* lpReserved            */ NULL
						                    , /* lpClass               */ NULL
						                    , /* lpcchClass            */ NULL
						                    , /* lpftLastWriteTime     */ NULL
						                    );
						if ( buffer.size() == 0 && buffer_size != 0 && error == ERROR_SUCCESS ) error = ERROR_MORE_DATA;
					}
					if ( error == ERROR_SUCCESS ) buffer.resize( buffer_size ), ++index;
					else if ( error == ERROR_NO_MORE_ITEMS ) key.reset();
					//else ERROR HANDLING HERE
					++index;
					return *this;
				}
				key_iterator  key_iterator::operator++( /* postfix */ int ) {
					key_iterator copy(*this);
					++*this;
					return copy;
				}
			}
		}
	}
}

#endif //defined( INDUSTRY_OS_WINDOWS )
