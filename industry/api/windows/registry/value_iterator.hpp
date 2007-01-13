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

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/key_wrapper.hpp>
#include <industry/api/windows/registry/value_reference.hpp>
#include <industry/api/windows/import.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <cassert>

namespace industry {
	namespace api {
		namespace windows {
			class registry_value_iterator {
			public:
				registry_value_iterator( boost::shared_ptr< registry_key_wrapper > const& key ): key(key), index(0), buffer() {} //start iterator (at beginning of list of values contained by the key)
				registry_value_iterator()                                                      : key()   , index(0), buffer() {} //end iterator

				typedef std::forward_iterator_tag  iterator_category;
				typedef DWORD                      size_type;
				typedef LONG                       difference_type;
				typedef registry_value_reference   reference;
				typedef registry_value_reference   pointer; //sic

				reference operator* () const { return registry_value_wrapper( key , std::basic_string< TCHAR >( buffer.begin() , buffer.end() ) ); }
				pointer   operator->() const { return registry_value_wrapper( key , std::basic_string< TCHAR >( buffer.begin() , buffer.end() ) ); }
				registry_value_iterator& operator++( /* prefix  */     );
				registry_value_iterator  operator++( /* postfix */ int );

				friend bool operator==( const registry_value_iterator & lhs , const registry_value_iterator & rhs ) {
					assert( lhs.key == rhs.key || !lhs.key || !rhs.key || !"Tried to compare registry_value_iterators of different ranges (keys)" );
					return (lhs.index == rhs.index) || (!lhs.key == !rhs.key);
				}
				friend bool operator!=( const registry_value_iterator & lhs , const registry_value_iterator & rhs ) {
					return !(lhs==rhs);
				}
			private:
				boost::shared_ptr< registry_key_wrapper >    key;
				DWORD                                        index;
				std::vector< TCHAR >                         buffer;
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_ITERATOR
