// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Created

#ifndef IG_INDUSTRY_TRAITS_CONTAINER
#define IG_INDUSTRY_TRAITS_CONTAINER

#include <industry/attributes/range.hpp>
#include <industry/sfinae.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace industry {
	namespace detail {
		template < typename C > static sfinae::one is_a_container_helper
			( typename C::container_category* = 0
			);
		template < typename C > static sfinae::one is_a_container_helper
			/* If you've got an error for any of the following lines, it's likely because one of the typedefs
			 * of C was unexpectedly a reference.  Please specialize industry::is_a_container< C >, with the
			 * static boolean ::value representing wheither or not C is in fact a container.  Thanks!
			 */
			( typename C::value_type*       = 0
			, typename C::iterator*         = 0
			, typename C::const_iterator*   = 0
			, typename boost::remove_reference< typename C::reference       >::type* = 0
			, typename boost::remove_reference< typename C::const_reference >::type* = 0
			, typename C::pointer*          = 0
			, typename C::difference_type*  = 0
			, typename C::size_type*        = 0
			/* If you've got an error for any of the following lines, it's because you have all the typedefs
			 * of a container, but are missing one of the required functions, or it is of an unexpected type.
			 * please specialize industry::is_a_container< C >, with the static boolean ::value representing
			 * wheither or not C is in fact a container.  Thanks!
			 */
			, typename void (C::*)()        = & C::clear
			, typename C::iterator (C::*)() = & C::begin
			, typename C::iterator (C::*)() = & C::end
			/* End of function signature check area */
			);
		template < typename C > static sfinae::two is_a_container_helper( ... );
	}

	template < typename Container > struct is_a_container {
		static const bool value = (sizeof(sfinae::one) == sizeof(detail::is_a_container_helper<Container>()));
	};
	template < typename Iterator > class range;
	template < typename Iterator > struct is_a_container<       range< Iterator > > { static const bool value = false; };
	template < typename Iterator > struct is_a_container< const range< Iterator > > { static const bool value = false; };

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

#endif //ndef IG_INDUSTRY_TRAITS_CONTAINER
