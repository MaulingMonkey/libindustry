// Lib Industry
//
//  Copyright Sean Kent 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  $LastChangedDate$ - $Author$
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
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

namespace industry {
	template<typename Fn> struct function_traits;

#define BOOST_PP_ITERATION_LIMITS (0, 25)
#define BOOST_PP_FILENAME_1       <industry/traits/function_traits.hpp>
#include BOOST_PP_ITERATE()

}
#endif //IG_INDUSTRY_FUNCTION_TRAITS

#else // BOOST_PP_IS_ITERATING

#define n BOOST_PP_ITERATION()

#define DECL(z, n, _) typedef A##n arg##n##_type;

template<class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (*)(BOOST_PP_ENUM_PARAMS(n,A))> {
	typedef R (signature)(BOOST_PP_ENUM_PARAMS(n,A)) ;
	static const bool is_member_function = false;
	static const unsigned int arity = n;
	static const bool has_var_arg = false;

	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, nil)
};

template<class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (*)(BOOST_PP_ENUM_PARAMS(n,A) ...)> {
	typedef R (signature)(BOOST_PP_ENUM_PARAMS(n,A) ...) ;
	static const bool is_member_function = false;
	static const unsigned int arity = n;
	static const bool has_var_arg = true;

	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (BOOST_PP_ENUM_PARAMS(n,A))> {
	typedef R (signature)(BOOST_PP_ENUM_PARAMS(n,A)) ;
	static const bool is_member_function = false;
	static const unsigned int arity = n;
	static const bool has_var_arg = false;

	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (BOOST_PP_ENUM_PARAMS(n,A) ...)> {
	typedef R (signature)(BOOST_PP_ENUM_PARAMS(n,A) ...) ;
	static const bool is_member_function = false;
	static const unsigned int arity = n;
	static const bool has_var_arg = true;

	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class C, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (C::*)(BOOST_PP_ENUM_PARAMS(n,A)) const> {
	typedef R (signature)(const C* BOOST_PP_ENUM_TRAILING_PARAMS(n, A));
	static const bool is_member_function = true;
	static const unsigned int arity = n;
	static const bool has_var_arg = false;

	typedef C class_type;
	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class C, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (C::*)(BOOST_PP_ENUM_PARAMS(n,A) ...) const> {
	typedef R (signature)(const C* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ...);
	static const bool is_member_function = true;
	static const unsigned int arity = n;
	static const bool has_var_arg = true;

	typedef C class_type;
	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class C, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (C::*)(BOOST_PP_ENUM_PARAMS(n,A))> {
	typedef R (signature)(C* BOOST_PP_ENUM_TRAILING_PARAMS(n, A)) ;
	static const bool is_member_function = true;
	static const unsigned int arity = n;
	static const bool has_var_arg = false;

	typedef C class_type;
	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

template<class C, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class A)>
struct function_traits <R (C::*)(BOOST_PP_ENUM_PARAMS(n,A) ...)> {
	typedef R (signature)(C* BOOST_PP_ENUM_TRAILING_PARAMS(n, A) ...) ;
	static const bool is_member_function = true;
	static const unsigned int arity = n;
	static const bool has_var_arg = true;

	typedef C class_type;
	typedef R result_type;
	BOOST_PP_REPEAT_FROM_TO(0, n, DECL, T)
};

#undef DECL
#undef n

#endif // BOOST_PP_IS_ITERATING
