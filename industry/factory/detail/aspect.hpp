// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jun 17, 2006 - Added key to template param list (compatibility break!)
// May 31, 2006 - Created

#ifndef BOOST_PP_IS_ITERATING
  #ifndef IG_INDUSTRY_FACTORY_DETAIL_ASPECT
  #define IG_INDUSTRY_FACTORY_DETAIL_ASPECT
    #include <industry/factory/exception.hpp>
    #include <industry/type.hpp>
    #include <boost/preprocessor/iteration.hpp>
    #include <boost/preprocessor/repetition.hpp>

namespace industry {
	namespace factory_detail {
		template < typename Base , typename Method > class factory_aspect;
		
		#define BOOST_PP_ITERATION_LIMITS   (0,INDUSTRY_METHOD_ARGUMENTS_LIMIT)
		#define BOOST_PP_FILENAME_1         <industry/factory/detail/aspect.hpp>
		#include BOOST_PP_ITERATE()
	}
}

  #endif //ndef IG_INDUSTRY_FACTORY_DETAIL_ASPECT
#else //BOOST_PP_IS_ITERATING

#define N                      BOOST_PP_ITERATION()
#if N == 0
  #define TRAILING_TYPENAME_AN
  #define AN                     void
  #define TRAILING_A_ARGN
  #define ARGN
#else
  #define TRAILING_TYPENAME_AN   BOOST_PP_ENUM_TRAILING_PARAMS(N,typename A)
  #define AN                     BOOST_PP_ENUM_PARAMS(N,A)
  #define TRAILING_A_ARGN        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N,A,arg)
  #define ARGN                   BOOST_PP_ENUM_PARAMS(N,arg)
#endif

		template < typename KeyT , typename Interface , typename Methods   TRAILING_TYPENAME_AN >
		class factory_aspect< factory_base< KeyT , Interface , Methods > , void ( AN ) > {
			typedef Interface                                                             interface_type;
			typedef factory_base< KeyT , Interface , Methods >                            base_type;
			typedef typename interface_type::template method< void ( AN ) >::return_type  return_type;
			typedef typename interface_type::template method< void ( AN ) >::type         method_type;
			typedef method_type *                                                         method_ptr_type;
		public:
			return_type create( KeyT key  TRAILING_A_ARGN ) {
				base_type & self = static_cast< base_type & >( *this );
				if ( !self.tables[ key ] ) throw bad_factory_type();
				return (*(static_cast< method_ptr_type >( self.tables[ key ]->entries)))( ARGN );
			}
		};
		
#undef N
#undef TRAILING_TYPENAME_AN
#undef AN
#undef TRAILING_A_ARGN
#undef ARGN

#endif //BOOST_PP_IS_ITERATING
