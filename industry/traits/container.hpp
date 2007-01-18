// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Created

#include <industry/attributes/range.hpp>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace industry {
	template < typename Container > class container_traits;

	template < typename Container > class sequence_container_traits {
	public:
		template < typename Range >
		static void assign( Container& container , const Range& range ) {
			using namespace industry::attributes::range;
			container.clear();
			container.insert(container.end(),begin(range),end(range));
		}
	};

	template < typename Container > class associative_container_traits {
	public:
		template < typename Range >
		static void assign( Container& container , const Range& range ) {
			using namespace industry::attributes::range;
			container.clear();
			container.insert(begin(range),end(range));
		}
	};

	template < typename Type , typename Alloc > class container_traits< std::deque< Type , Alloc > >
		: public sequence_container_traits< std::deque< Type , Alloc > >
	{};

	template < typename Type , typename Alloc > class container_traits< std::list< Type , Alloc > >
		: public sequence_container_traits< std::list< Type , Alloc > >
	{};

	template < typename Key , typename Value , typename Compare , typename Alloc > class container_traits< std::map< Key , Value , Compare , Alloc > >
		: public associative_container_traits< std::map< Key , Value , Compare , Alloc > >
	{};

	template < typename Key , typename Compare , typename Alloc > class container_traits< std::set< Key , Compare , Alloc > >
		: public associative_container_traits< std::set< Key , Compare , Alloc > >
	{};

	template < typename Type , typename Alloc > class container_traits< std::vector< Type , Alloc > >
		: public sequence_container_traits< std::vector< Type , Alloc > >
	{};
}
