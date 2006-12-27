// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.hpp => industry/factory.hpp
// Jun 17, 2006 - Added key to template param list (compatibility break!)
// May 27, 2006 - Changed factory to factory_base, new factory allows more syntax & interface options
// May 21, 2006 - Created

#ifndef IG_INDUSTRY_FACTORY
#define IG_INDUSTRY_FACTORY

#ifndef INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT
#define INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT INDUSTRY_METHOD_ARGUMENTS_LIMIT
#endif //ndef INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT

namespace industry {
	template < typename T > struct factory_interface { typedef T type; };
	template < typename KeyT , typename InterfaceT , typename MethodsT > class factory_base;
}

#include <industry/methods.hpp>
#include <industry/factory/detail/base.hpp>
#include <industry/factory/exception.hpp>

namespace industry {
	/* examples:
	
	industry::factory< boost::any , industry::methods< void ( void ) , void ( unsigned ) > >  example;
	industry::factory< boost::any , void ( void ) , void ( unsigned ) > example;
	
	*/

#define TYPENAME_NIL_Tn          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( INDUSTRY_METHODS_LIMIT , typename T , nil )
#define TYPENAME_Tn              BOOST_PP_ENUM_PARAMS( INDUSTRY_METHODS_LIMIT , typename T )
#define Tn                       BOOST_PP_ENUM_PARAMS( INDUSTRY_METHODS_LIMIT , T )
#define NIL_instance(z,n,u)      nil
#define TRAILING_NILn_MINUS_ONE  BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB( INDUSTRY_METHODS_LIMIT , 1 ) , NIL_instance , ~ )
#define IMPLEMENTATION           : public factory_base< KeyT , typename factory_interface< InterfaceT >::type , methods< Tn > > {}
	template < typename KeyT , typename InterfaceT , TYPENAME_NIL_Tn > class factory      IMPLEMENTATION;
	template < typename KeyT , typename InterfaceT , TYPENAME_Tn >
	class factory< KeyT , InterfaceT , methods< Tn >  TRAILING_NILn_MINUS_ONE >  IMPLEMENTATION;

#undef TYPENAME_NIL_Tn
#undef TYPENAME_Tn
#undef Tn
#undef NIL_instance
#undef TRAILING_NILn_MINUS_ONE
#undef IMPLEMENTATION
}

#endif //ndef IG_INDUSTRY_FACTORY
