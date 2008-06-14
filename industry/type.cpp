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
#include <vector>

namespace industry {
	namespace {
		typedef std::map< const std::type_info*, unsigned > types_t;
		typedef types_t::value_type type_entry;

		std::vector< const std::type_info* > infos;
		types_t types;
	}

	const char* type::name() const throw() {
		return infos[info]->name();
	}

	type::type( const std::type_info& src ) {
		types_t::iterator entry = types.find(&src);
		if ( entry != types.end() ) {
			info = entry->second;
			return;
		}

#ifndef NO_TYPEINFO_DUPLICATES
		// Search for duplicates
		BOOST_FOREACH( type_entry& t, types ) {
			if ( *t.first == src ) {
				types.insert(entry,std::make_pair(&src,t.second));
				info = t.second;
				return;
			}
		}
#endif

		// register new entry
		infos.push_back(&src);
		unsigned newid = static_cast<unsigned>(infos.size());
		types.insert(entry,std::make_pair(&src,newid));
		info = newid;
	}
}
