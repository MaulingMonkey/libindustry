// Copyright (c) 2007-2008 Sean Kent
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy: washuhibiki $ - $LastChangedDate: 2008-05-13 13:45:12 -0700 (Tue, 13 May 2008) $

#if !BOOST_PP_IS_ITERATING
#ifndef IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CONSTRUCTOR_REGISTRY
#define IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CONSTRUCTOR_REGISTRY

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/for_each_product.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <map>
#include <string>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 ) // unreferenced formal parameters
#pragma warning( disable: 4101 ) // unreferenced formal parameters
#endif

namespace industry { namespace languages { namespace ruby { namespace detail {
	template<class T, unsigned int A>
	struct constructor_registry;

	template<class T, class Sig, unsigned int A>
	struct constructor_registry_impl {
	};

#	define BOOST_PP_ITERATION_LIMITS (0, 17)
#	define BOOST_PP_FILENAME_1       <industry/languages/ruby/detail/constructor_registry.hpp>
#	include BOOST_PP_ITERATE()

}}}}
#endif//IG_INDUSTRY_LANGAUGES_RUBY_DETAIL_CONSTRUCTOR_REGISTRY
#else
#define n BOOST_PP_ITERATION()
#define FUNCTION_ARG_TYPE(i) BOOST_PP_CAT(typename func_type::arg, BOOST_PP_CAT(BOOST_PP_INC(i),_type))
#define DO_TEST_FUNC_ARG(z,i,data) if (!ruby_value<FUNCTION_ARG_TYPE(i)>::is_a(argv[i])) return false;
#define DO_WRAP_FUNC_ARG(z,i,data) (ruby_value<FUNCTION_ARG_TYPE(i)>::from(argv[i]))

template<class T, class Sig>
struct constructor_registry_impl<T, Sig, n> {
	typedef boost::function<Sig> func_type;
	static void reg() {
		std::vector<constructor_registry_entry>& m = constructor_registry<T, n>::get_constructors();
		constructor_registry_entry entry = { &test_match , &f };
		m.push_back(entry);
	}
	
	static bool test_match(int argc, VALUE* argv, VALUE self) {
		BOOST_PP_REPEAT(n, DO_TEST_FUNC_ARG, BOOST_PP_EMPTY); // returns false if mismatch
		return true;
	}
	static VALUE f(int argc, VALUE* argv, VALUE self) {
		T* ptr = ruby_value<T*>::from(self);
		new (ptr) T(BOOST_PP_ENUM(n, DO_WRAP_FUNC_ARG, BOOST_PP_EMPTY));
		instance_registry<T>::ruby_initialized(ptr,self); // inject self
		instance_registry<T>::register_ruby_owned(ptr);
		return self;
	}
};

#undef n
#undef DO_WRAP_VALUE
#undef DO_WRAP_VALUE_MEMBER
#undef FUNCTION_ARG_TYPE
#endif
