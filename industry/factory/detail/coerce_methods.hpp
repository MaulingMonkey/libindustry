// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.detail.coerce.methods.hpp => industry/factory/detail/coerce_methods.hpp
// May 31, 2006 - Created

#ifndef BOOST_PP_IS_ITERATING
  #ifndef IG_INDUSTRY_FACTORY_DETAIL_COERCE_METHODS
  #define IG_INDUSTRY_FACTORY_DETAIL_COERCE_METHODS
  #include <boost/preprocessor/repetition.hpp>
  #include <boost/preprocessor/iteration.hpp>
  #include <industry/config.hpp>
  #include <industry/methods.hpp>
  #include <industry/multitype.hpp>

namespace industry {
	namespace factory_detail {
		template < typename Interface , typename Methods > struct coerce_methods;
		
		#define BOOST_PP_ITERATION_LIMITS (1,INDUSTRY_METHODS_LIMIT)
		#define BOOST_PP_FILENAME_1 <industry/factory/detail/coerce_methods.hpp>
		#include BOOST_PP_ITERATE()
	}
}

  #endif //ndef IG_INDUSTRY_FACTORY_ASPECT
#else //BOOST_PP_IS_ITERATING
  #define N                              BOOST_PP_ITERATION()
  #define TYPENAME_MN                    BOOST_PP_ENUM_PARAMS(N,typename M)
  #define MN                             BOOST_PP_ENUM_PARAMS(N,M)
  #define VTABLE_FPTR_entry(z,n,unused)  typename Interface::template method< M ## n >::type
  #define VTABLE_FPTRN                   BOOST_PP_ENUM(N,VTABLE_FPTR_entry,~)

		template < typename Interface , TYPENAME_MN >
		struct coerce_methods< Interface , methods< MN > > {
			typedef methods< VTABLE_FPTRN > type;
		};

  #undef N
  #undef TYPENAME_MN
  #undef MN
  #undef VTABLE_FPTR_entry
  #undef VTABLE_FPTRN
#endif //ndef IG_INDUSTRY_FACTORY_DETAIL_COERCE_METHODS
