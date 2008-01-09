// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.detail.vtable.hpp => industry/factory/detail/vtable.hpp
// May 31, 2006 - Switched to file iteration
// May 27, 2006 - Created

#ifndef BOOST_PP_IS_ITERATING
  #ifndef IG_INDUSTRY_FACTORY_DETAIL_VTABLE
  #define IG_INDUSTRY_FACTORY_DETAIL_VTABLE
    #include <boost/preprocessor/repetition.hpp>
    #include <boost/preprocessor/iteration.hpp>
	#include <industry/config.hpp>

namespace industry {
	namespace factory_detail {
		template < typename Methods > struct vtable;
		
		#define BOOST_PP_ITERATION_LIMITS  (1, INDUSTRY_METHODS_LIMIT)
		#define BOOST_PP_FILENAME_1        <industry/factory/detail/vtable.hpp>
		#include BOOST_PP_ITERATE()
	}
}

  #endif //ndef IG_INDUSTRY_FACTORY_DETAIL_VTABLE

#else // BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()
#define TYPENAME_Mn               BOOST_PP_ENUM_PARAMS( n , typename M )
#define Mn                        BOOST_PP_ENUM_PARAMS( n , M )
#define MPTR_impl(z,n,unused)     M ## n *
#define MPTRn                     BOOST_PP_ENUM( n , MPTR_impl , ~ )
#define VTABLE_entry(z,n,unused)  table.entries = & Aspect< T , M ## n >::create

		template < TYPENAME_Mn > struct vtable< methods< Mn > > {
			multitype< MPTRn > entries;	
			template < template < typename , typename > class Aspect , typename T >
			static vtable * create( void ) {
				static vtable table;
				BOOST_PP_ENUM(n, VTABLE_entry , ~);
				return &table;
			}
		};

#undef TYPENAME_Mn
#undef Mn
#undef MPTRn
#undef MPTR_impl
#undef VTABLE_entry

#endif //ndef BOOST_PP_IS_ITERATING
