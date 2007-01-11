// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Created

#ifndef IG_INDUSTRY_CONTAINERS
#define IG_INDUSTRY_CONTAINERS

#include <industry/traits/container.hpp>
#include <industry/algorithm/push.hpp>

namespace industry {
	namespace containers {
		template < typename StdContainer >
		class container {
			typedef container< StdContainer > self;
		public:
			//self& operator=( 
		};

		template < typename StdContainer >
		class container_reference {
			typedef container_reference< StdContainer > self;
			
			StdContainer& reference;
		public:
			container_reference( StdContainer& reference ): reference(reference) {}

			template < typename Iterator >
			self& operator=( const range< Iterator >& range ) {
				container_traits< StdContainer >::assign( reference , range );
			}
		};

		template < typename StdContainer >
		container_reference< StdContainer > as_container( StdContainer& container ) {
			return container_reference< StdContainer >( container );
		}

		template < typename StdContainer >
		container_reference< const StdContainer > as_container( const StdContainer& container ) {
			return container_reference< const StdContainer >( container );
		}
	}
}

#endif //ndef IG_INDUSTRY_CONTAINERS
