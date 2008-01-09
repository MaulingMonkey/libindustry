// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 13, 2007 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/detail/key_data.hpp>
#include <industry/api/windows/registry/value.hpp>
#include <industry/api/windows/import.hpp>
#include <industry/api/windows/tstring.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <cassert>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				class value_iterator {
				public:
					value_iterator( const detail::key_data_ptr& key ): key(key), index(0), buffer() {} //start iterator (at beginning of list of values contained by the key)
					value_iterator()                                 : key()   , index(0), buffer() {} //end iterator

					typedef std::forward_iterator_tag  iterator_category;
					typedef DWORD                      size_type;
					typedef LONG                       difference_type;
					typedef value                      value_type;
					typedef value                      reference;
					typedef value                      pointer; //sic

					reference operator* () const { return value( key , tstring( buffer.begin() , buffer.end() ) ); }
					pointer   operator->() const { return value( key , tstring( buffer.begin() , buffer.end() ) ); }
					value_iterator& operator++( /* prefix  */     );
					value_iterator  operator++( /* postfix */ int );

					friend bool operator==( const value_iterator & lhs , const value_iterator & rhs ) {
						assert( lhs.key == rhs.key || !lhs.key || !rhs.key || !"Tried to compare registry_value_iterators of different ranges (keys)" );
						return ( lhs.key && rhs.key && (lhs.index == rhs.index) ) || (!lhs.key == !rhs.key);
					}
					friend bool operator!=( const value_iterator & lhs , const value_iterator & rhs ) {
						return !(lhs==rhs);
					}
				private:
					detail::key_data_ptr    key;
					DWORD                   index;
					std::vector< TCHAR >    buffer;
				};
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR
