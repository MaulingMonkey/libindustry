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

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/detail/key_data.hpp>
#include <industry/api/windows/registry/value.hpp>
#include <industry/api/windows/tstring.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				class key_iterator;
				class value_iterator;
				class key {
					detail::key_data_ptr impl;

					void  initialize  ( const detail::key_data_ptr & parent , const tstring & name );
					key   select_key  ( const tstring & name ) const;
					value select_value( const tstring & name ) const;
				public:
					key();
					key( HKEY );
					key( const key                  & parent, const tstring & name );
					key( const detail::key_data_ptr & parent, const tstring & name );
					~key();

					template < typename CharT > key( const key & parent , const std::basic_string< CharT > & name ) { initialize(parent,to_tstring(name)); }
					template < typename CharT > value operator[]( const std::basic_string< CharT > & name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > value operator% ( const std::basic_string< CharT > & name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > key   operator/ ( const std::basic_string< CharT > & name ) const { return select_key  (to_tstring(name)); }
					template < typename CharT > key & operator/=( const std::basic_string< CharT > & name ) { return *this = *this / name; }

					template < typename CharT > key( const key & parent , const CharT* name ) { initialize(parent,to_tstring(name)); }
					template < typename CharT > value operator[]( const CharT* name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > value operator% ( const CharT* name ) const { return select_value(to_tstring(name)); }
					template < typename CharT > key   operator/ ( const CharT* name ) const { return select_key  (to_tstring(name)); }
					template < typename CharT > key & operator/=( const CharT* name ) { return *this = *this / name; }

					boost::iterator_range< key_iterator   > keys  () const;
					boost::iterator_range< value_iterator > values() const;
				};
				template < typename T > bool operator==( const key & k , const T & value ) { return k % "" == value; }
				template < typename T > bool operator==( const T & value , const key & k ) { return k % "" == value; }
				template < typename T > bool operator!=( const key & k , const T & value ) { return !(k == value); }
				template < typename T > bool operator!=( const T & value , const key & k ) { return !(k == value); }

				template < typename CharT >
				inline std::basic_ostream< CharT > & operator<<( std::basic_ostream< CharT > & os , const key & k ) { return os << (k % ""); }
			}
		}
	}
}

#include <industry/api/windows/registry/key_iterator.hpp>
#include <industry/api/windows/registry/value_iterator.hpp>

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_KEY
