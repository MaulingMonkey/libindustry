// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_UTILITY_NEW_SHARED_PTR
#define IG_INDUSTRY_UTILITY_NEW_SHARED_PTR

#include <boost/preprocessor.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

namespace industry {
	namespace utility {
/*---------------------------------------------------------------------------------------------------------------------*/
#define DO_ITERATION(z,n,unused)                                                                                        \
		template < typename T  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename A) >                                            \
		boost::shared_ptr< T > new_shared_ptr(  BOOST_PP_ENUM_BINARY_PARAMS(n,const A,& arg)  ) {                       \
			return boost::shared_ptr< T >( new T( BOOST_PP_ENUM_PARAMS(n,arg) ) );                                      \
		}                                                                                                               \
		                                                                                                                \
		template < typename T , typename D  BOOST_PP_ENUM_TRAILING_PARAMS(n,typename A) >                               \
		boost::shared_ptr< T > new_shared_ptr_with_deleter(  BOOST_PP_ENUM_BINARY_PARAMS(n,const A,& arg)  BOOST_PP_COMMA_IF(n)  D d ) { \
			return boost::shared_ptr< T >( new T( BOOST_PP_ENUM_PARAMS(n,arg) ) BOOST_PP_COMMA_IF(n) d );               \
		}                                                                                                               \
/*---------------------------------------------------------------------------------------------------------------------*/
		BOOST_PP_REPEAT_FROM_TO(0,10,DO_ITERATION,~)                                                                   //
#undef DO_ITERATION                                                                                                    //
/*---------------------------------------------------------------------------------------------------------------------*/
	}
}

#endif //ndef IG_INDUSTRY_UTILITY_NEW_SHARED_PTR
