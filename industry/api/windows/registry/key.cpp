// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 17, 2007 - registry_key_reference -> registry::key, centralized logic around tstring
// Jan  9, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#include <industry/config.hpp>

#if !defined( INDUSTRY_OS_WINDOWS )
//Nothing

#elif defined( _MSC_VER ) && !defined( _MSC_EXTENSIONS )
#include <boost/preprocessor/stringize.hpp>
#pragma message( __FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ") : warning: industry/api/windows/* will not be compiled without language extensions (windows.h pukes)" )

#else
#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/errors.hpp>
#include <industry/api/windows/registry/key.hpp>
#include <industry/api/windows/import.hpp>
#include <industry/iterator/cast.hpp>
#include <limits>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				key::key()
					: impl()
				{
				}
				key::key( HKEY unmanaged_key )
					: impl( new detail::key_data( unmanaged_key , false ) )
				{
				}
				key::~key() {
					//full definition of registry_key_reference::implementation now available for impl (shared_ptr's) destructor
				}
				void key::initialize( const key & parent , const tstring & name ) {
					if (!parent.impl) throw null_key_error();
					HKEY result;
					LONG error = ::RegOpenKey( /* hKey      */ *parent.impl
											 , /* lpSubKey  */ name.c_str()
											 , /* phkResult */ & result
											 );
					throw_key_error( error );
					impl.reset( new detail::key_data( result , true ) );
				}
				key   key::select_key  ( const tstring & name ) const {
					return key( *this , name );
				}
				value key::select_value( const tstring & name ) const {
					return value( impl , name );
				}
				industry::range< value_iterator > each_value ( const key & k ) {
					return industry::range< value_iterator >( value_iterator( k.impl ) , value_iterator() );
				}
#if 0
				namespace {
					struct RegEnumKeyExCaller {
						HKEY key;
						std::vector< wchar_t > name_buffer;

						RegEnumKeyExCaller( HKEY key ) : key( key ) {}

						LONG operator()( DWORD index ) {
							LONG  error = ERROR_MORE_DATA;
							DWORD name_buffer_size = 16;

							while ( error == ERROR_MORE_DATA ) {
								name_buffer_size += 1; //when [out], dosn't include the terminating _T('\0') - however [in] expects it, compensate for this Win32 API idiotcyncracy.
								name_buffer.resize( name_buffer_size );
								if ( name_buffer.capacity() > name_buffer.size() ) name_buffer.resize( name_buffer.capacity() );
								assert( name_buffer.size() >= name_buffer_size );
								assert( name_buffer.size() <= std::numeric_limits< DWORD >::max() );
								name_buffer_size = DWORD( name_buffer.size() );

								error = RegEnumKeyExW( /* hKey               */ key
													 , /* dwIndex            */ index
													 , /* lpName             */ name_buffer.empty() ? NULL : &name_buffer[0]
													 , /* lpcName   [in/out] */ &name_buffer_size
													 , /* lpReserved         */ NULL
													 , /* lpClass            */ NULL
													 , /* lpcClass           */ NULL
													 , /* lpftLastWriteTime  */ NULL
													 );
							   if ( name_buffer.size() == 0 && name_buffer_size != 0 && error == ERROR_SUCCESS ) error = ERROR_MORE_DATA;
							}
							if ( error == ERROR_SUCCESS ) name_buffer.resize( name_buffer_size );
							return error;
						}
					};
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
				}
				std::vector< registry_key_reference   > find_         ( const registry_key_reference & key , const std::string & key_name   ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumKeyExCaller enum_key( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_key_reference > results;
					std::string buffer;

					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_key( id );
						buffer.assign( enum_key.name_buffer.begin() , enum_key.name_buffer.end() );
						if ( buffer.find( key_name ) != std::string::npos ) results.push_back( key / buffer );
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_value_reference > find_value_   ( const registry_key_reference & key , const std::string & value_name ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumValueCaller enum_value( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_value_reference > results;
					std::string buffer;
					
					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_value( id );
						buffer.assign( enum_value.name_buffer.begin() , enum_value.name_buffer.end() );
						if ( buffer.find( value_name ) != std::string::npos ) results.push_back( key % buffer );
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_key_reference   > find_if_      ( const registry_key_reference & key , const boost::function< bool ( const char* , const char* ) > & predicate ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumKeyExCaller enum_key( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_key_reference > results;
					std::vector< char > buffer;

					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_key( id );
						buffer.assign( static_iterator_cast< char >( enum_key.name_buffer.begin() )
									 , static_iterator_cast< char >( enum_key.name_buffer.end()   )
									 );
						if ( predicate( &buffer[0] , &buffer[0] + buffer.size() ) ) {
							results.push_back( key / std::string( enum_key.name_buffer.begin() , enum_key.name_buffer.end() ) );
						}
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_value_reference > find_value_if_( const registry_key_reference & key , const boost::function< bool ( const char* , const char* ) > & predicate ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumValueCaller enum_value( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_value_reference > results;
					std::vector< char > buffer;
					
					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_value( id );
						buffer.assign( static_iterator_cast< char >( enum_value.name_buffer.begin() )
									 , static_iterator_cast< char >( enum_value.name_buffer.end()   )
									 );
						if ( predicate( &buffer[0] , &buffer[0] + buffer.size() ) ) {
							results.push_back( key % std::string( enum_value.name_buffer.begin() , enum_value.name_buffer.end() ) );
						}
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
	#if defined( UNICODE )
				registry_key_reference::registry_key_reference( const registry_key_reference & parent , const std::wstring & name ) {
					if ( ! parent.key ) throw null_registry_key_error();
					HKEY result;
					LONG error = ::RegOpenKeyW( /* hKey      */ parent.key->key
											  , /* lpSubKey  */ name.c_str()
											  , /* phkResult */ & result
											  );
					throw_registry_key_error( error );
					key.reset( new registry_key_wrapper( result , true ) );
				}
				registry_value_reference registry_key_reference::operator[]( const std::wstring & name ) const {
					return registry_value_reference( registry_value_wrapper( key , tstring( name.begin() , name.end() ) ) );
				}
				registry_value_reference registry_key_reference::operator% ( const std::wstring & name ) const {
					return registry_value_reference( registry_value_wrapper( key , tstring( name.begin() , name.end() ) ) );
				}
				registry_key_reference   registry_key_reference::operator/ ( const std::wstring & name ) const {
					return registry_key_reference( *this , name );
				}

				//TODO:  Implement this shit!
				//friend std::vector< registry_key_reference   > find         ( const registry_key_reference & , const std::wstring & key_name   );
				//friend std::vector< registry_value_reference > find_value   ( const registry_key_reference & , const std::wstring & value_name );
				//friend std::vector< registry_key_reference   > find_if      ( const registry_key_reference & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & key_name   );
				//friend std::vector< registry_value_reference > find_value_if( const registry_key_reference & , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & value_name );
				std::vector< registry_key_reference   > find_         ( const registry_key_reference & key , const std::wstring & key_name   ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumKeyExCaller enum_key( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_key_reference > results;
					std::wstring buffer;

					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_key( id );
						buffer.assign( enum_key.name_buffer.begin() , enum_key.name_buffer.end() );
						if ( buffer.find( key_name ) != std::wstring::npos ) results.push_back( key / buffer );
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_value_reference > find_value_   ( const registry_key_reference & key , const std::wstring & value_name ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumValueCaller enum_value( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_value_reference > results;
					std::wstring buffer;
					
					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_value( id );
						buffer.assign( enum_value.name_buffer.begin() , enum_value.name_buffer.end() );
						if ( buffer.find( value_name ) != std::wstring::npos ) results.push_back( key % buffer );
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_key_reference   > find_if_      ( const registry_key_reference & key , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & predicate ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumKeyExCaller enum_key( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_key_reference > results;
					std::vector< wchar_t > buffer;

					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_key( id );
						buffer.assign( static_iterator_cast< wchar_t >( enum_key.name_buffer.begin() )
									 , static_iterator_cast< wchar_t >( enum_key.name_buffer.end()   )
									 );
						if ( predicate( &buffer[0] , &buffer[0] + buffer.size() ) ) {
							results.push_back( key / std::wstring( enum_key.name_buffer.begin() , enum_key.name_buffer.end() ) );
						}
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
				std::vector< registry_value_reference > find_value_if_( const registry_key_reference & key , const boost::function< bool ( const wchar_t* , const wchar_t* ) > & predicate ) {
					//FIXME:  OMGWTFBBQrefactorme before applying changes plox
					RegEnumValueCaller enum_value( key.key->key );
					LONG error = ERROR_SUCCESS;
					std::vector< registry_value_reference > results;
					std::vector< wchar_t > buffer;
					
					for ( DWORD id = 0 ; error == ERROR_SUCCESS ; ++id ) {
						error = enum_value( id );
						buffer.assign( static_iterator_cast< wchar_t >( enum_value.name_buffer.begin() )
									 , static_iterator_cast< wchar_t >( enum_value.name_buffer.end()   )
									 );
						if ( predicate( &buffer[0] , &buffer[0] + buffer.size() ) ) {
							results.push_back( key % std::wstring( enum_value.name_buffer.begin() , enum_value.name_buffer.end() ) );
						}
					}
					if ( error != ERROR_NO_MORE_ITEMS ) throw_registry_error( error );
					return results;
				}
	#endif
	#endif
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
