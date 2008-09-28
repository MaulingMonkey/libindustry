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
#include <cassert>

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

	void init_types() {
		assert( type::id<void              >().key() == type::keys::void_               );
		assert( type::id<unsigned char     >().key() == type::keys::unsigned_char_      );
		assert( type::id<unsigned short    >().key() == type::keys::unsigned_short_     );
		assert( type::id<unsigned int      >().key() == type::keys::unsigned_int_       );
		assert( type::id<unsigned long     >().key() == type::keys::unsigned_long_      );
		assert( type::id<unsigned long long>().key() == type::keys::unsigned_long_long_ );
		assert( type::id<  signed char     >().key() == type::keys::  signed_char_      );
		assert( type::id<  signed short    >().key() == type::keys::  signed_short_     );
		assert( type::id<  signed int      >().key() == type::keys::  signed_int_       );
		assert( type::id<  signed long     >().key() == type::keys::  signed_long_      );
		assert( type::id<  signed long long>().key() == type::keys::  signed_long_long_ );
		assert( type::id<         char     >().key() == type::keys::         char_      );
		assert( type::id<         short    >().key() == type::keys::         short_     );
		assert( type::id<         int      >().key() == type::keys::         int_       );
		assert( type::id<         long     >().key() == type::keys::         long_      );
		assert( type::id<         long long>().key() == type::keys::         long_long_ );
		assert( type::id<  float           >().key() == type::keys::  float_            );
		assert( type::id<  double          >().key() == type::keys::  double_           );
	}

	type::type( const std::type_info& src ) {
		static bool types_initialized = false;
		if ( !types_initialized ) {
			types_initialized = true; // allow init_types() to call this ctor directly
			init_types();
		}

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
