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
				key::key( const key & parent, const tstring & name ) {
					initialize( parent.impl, name );
				}
				key::key( const detail::key_data_ptr & parent, const tstring & name ) {
					initialize( parent, name );
				}
				key::~key() {
					//full definition of registry_key_reference::implementation now available for impl (shared_ptr's) destructor
				}
				void key::initialize( const detail::key_data_ptr & parent , const tstring & name ) {
					if (!parent) throw null_key_error();
					HKEY result;
					LONG error = ::RegOpenKey( /* hKey      */ *parent
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
				boost::iterator_range< key_iterator   > key::keys  () const {
					return boost::iterator_range< key_iterator >( key_iterator( impl ) , key_iterator() );
				}
				boost::iterator_range< value_iterator > key::values() const {
					return boost::iterator_range< value_iterator >( value_iterator( impl ) , value_iterator() );
				}
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
