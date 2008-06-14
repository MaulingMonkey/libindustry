// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/type.hpp>
#include <boost/foreach.hpp>
#include <map>

namespace industry {
#ifndef NO_TYPEINFO_DUPLICATES
	type::type( const std::type_info& src ) {
		typedef std::map< const std::type_info*, const std::type_info* > type_list_t;
		static type_list_t types;

		const std::type_info*& entry = types[&src];
		info = entry;
		if (info) return;

		// Search for duplicates
		BOOST_FOREACH( type_list_t::value_type& t, types ) {
			if ( *t.first == src ) {
				entry = t.first; // register duplicate
				info = entry;
				return;
			}
		}

		entry = &src; // register new entry
		info = entry;
		return;
	}
#endif
}
