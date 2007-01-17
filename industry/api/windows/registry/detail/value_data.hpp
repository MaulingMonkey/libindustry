// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_VALUE_DATA
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_VALUE_DATA

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/detail/key_data.hpp>
#include <industry/api/windows/tstring.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				namespace detail {
					class value_data {
					public:
						value_data() {}

						key_data_ptr key;
						tstring name;

						value_data( const key_data_ptr& key, const tstring& name ): key(key), name(name) {}
					};

					typedef boost::shared_ptr< value_data > value_data_ptr;
				}
			}
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_DETAIL_VALUE_DATA
