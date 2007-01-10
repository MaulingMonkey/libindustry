// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.interface.aspect.gen.hpp => industry/factory/interface_aspect_gen.hpp
// May 31, 2006 - Created

// <= INDUSTRY_FACTORY_ASPECT_FILENAME

#ifndef IG_INDUSTRY_FACTORY_INTERFACE_ASPECT_GEN_COMMON
#define IG_INDUSTRY_FACTORY_INTERFACE_ASPECT_GEN_COMMON
  #include <boost/preprocessor/repetition.hpp>
  #include <boost/preprocessor/iteration.hpp>
  #include <industry/config.hpp>
#endif //ndef IG_INDUSTRY_FACTORY_INTERFACE_ASPECT_GEN_COMMON

#ifndef BOOST_PP_IS_ITERATING
  #ifndef INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT
  #define INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT INDUSTRY_METHOD_ARGUMENTS_LIMIT
  #endif //ndef INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT
  #define BOOST_PP_ITERATION_LIMITS  (0, INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT)
  #define BOOST_PP_FILENAME_1        <industry/factory/interface_aspect_gen.hpp>
  #include BOOST_PP_ITERATE()
#else //BOOST_PP_IS_ITERATING
  #define INDUSTRY_FACTORY_INTERFACE_ASPECT_IS_GENERATING
  #define N                BOOST_PP_ITERATION()
  #if N == 0
    #define TRAILING_TYPENAME_AN
    #define TYPENAME_AN
	#define AN
	#define A_ARGN                 void
	#define ARGN
  #else
    #define TRAILING_TYPENAME_AN   BOOST_PP_ENUM_TRAILING_PARAMS(N,typename A)
    #define TYPENAME_AN            BOOST_PP_ENUM_PARAMS(N,typename A)
	#define AN                     BOOST_PP_ENUM_PARAMS(N,A)
    #define A_ARGN                 BOOST_PP_ENUM_BINARY_PARAMS(N,A,arg)
    #define ARGN                   BOOST_PP_ENUM_PARAMS(N,arg)
  #endif
  #include INDUSTRY_FACTORY_INTERFACE_ASPECT_FILENAME
  #undef INDUSTRY_FACTORY_INTERFACE_ASPECT_IS_GENERATING
  #undef N
  #undef TRAILING_TYPENAME_AN
  #undef TYPENAME_AN
  #undef AN
  #undef A_ARGN
  #undef ARGN
#endif //BOOST_PP_IS_ITERATING
