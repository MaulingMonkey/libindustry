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

#ifndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_WRAPPER
#define IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_WRAPPER

#include <industry/api/windows/registry/config.hpp>
#include <industry/api/windows/registry/key_wrapper.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace industry {
	namespace api {
		namespace windows {
			class registry_value_wrapper {
			public:
				boost::shared_ptr< registry_key_wrapper > key;
#if defined( INDUSTRY_UNICODE_ENABLED )
				std::wstring name;
				registry_value_wrapper( boost::shared_ptr< registry_key_wrapper > key , std::wstring name ) : key( key ) , name( name ) {}
#else
				std::string name;
				registry_value_wrapper( boost::shared_ptr< registry_key_wrapper > key , std::string  name ) : key( key ) , name( name ) {}
#endif
			};
		}
	}
}

#endif //ndef IG_INDUSTRY_API_WINDOWS_REGISTRY_VALUE_WRAPPER
