// Copyright (c) 2006-2007 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
#if !BOOST_PP_IS_ITERATING
#ifndef IG_INDUSTRY_FUNCTION_TRAITS
#define IG_INDUSTRY_FUNCTION_TRAITS
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/for_each_product.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

namespace industry {
	template<typename Fn> struct function_traits;

#define IG_INDUSTRY_FUNCTION_TRAIT_ITOR

#define BOOST_PP_ITERATION_LIMITS (0, 25)
#define BOOST_PP_FILENAME_1       "function_traits.hpp"
#include BOOST_PP_ITERATE()

#undef IG_INDUSTRY_FUNCTION_TRAIT_ITOR

#define BOOST_PP_ITERATION_LIMITS (0, 25)
#define BOOST_PP_FILENAME_1       "function_traits.hpp"
#include BOOST_PP_ITERATE()

}
#endif //IG_INDUSTRY_FUNCTION_TRAITS

#elif defined(IG_INDUSTRY_FUNCTION_TRAIT_ITOR)//BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

template<class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (BOOST_PP_ENUM_PARAMS(n,A))> {
	typedef R (signature)(BOOST_PP_ENUM_PARAMS(n,A)) ;
};

#undef n

#else //!IG_INDUSTRY_FUNCTION_TRAIT_ITOR

#define n BOOST_PP_ITERATION()

template<class C, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (C::*)(BOOST_PP_ENUM_PARAMS(n,A))> {
	typedef R (signature)(C* BOOST_PP_ENUM_TRAILING_PARAMS(n, A)) ;
};

#undef n

#endif