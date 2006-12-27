// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.detail.base.hpp => industry/factory/detail/base.hpp
// Jun 17, 2006 - Switched to file iteration
// May 31, 2006 - industry.factory.any => industry.factory.detail.base
// May 31, 2006 - Parred down to main and aspect specializations
// May ??, 2006 - Slowly generalized classes & deleted their defunct versions
// May 21, 2006 - Created

#ifndef BOOST_PP_IS_ITERATING
  #ifndef IG_INDUSTRY_FACTORY_DETAIL_BASE
  #define IG_INDUSTRY_FACTORY_DETAIL_BASE
    #include <industry/config.hpp>
    #include <industry/factory.hpp>
    #include <industry/factory/detail/aspect.hpp>
    #include <industry/factory/detail/coerce_methods.hpp>
    #include <industry/factory/detail/vtable.hpp>
    #include <industry/factory/interface_aspects.hpp>
    #include <industry/methods.hpp>
    #include <industry/inherit.hpp>
    #include <industry/multitype.hpp>
    #include <industry/type.hpp>
    #include <boost/any.hpp>
    #include <boost/preprocessor/iteration.hpp>
    #include <boost/preprocessor/repetition.hpp>
    #include <map>
    #include <cassert>

    #define INDUSTRY_FACTORY_ASPECT_CREATE_use(z,n,limit) \
		using factory_detail::factory_aspect< factory_base< key_type , Interface , methods< BOOST_PP_ENUM_PARAMS(limit,M) > > , M ## n >::create;
    
    #define BOOST_PP_ITERATION_LIMITS (1,INDUSTRY_METHODS_LIMIT)
    #define BOOST_PP_FILENAME_1 <industry/factory/detail/base.hpp>
    #include BOOST_PP_ITERATE()
    
    #undef INDUSTRY_FACTORY_ASPECT_CREATE_use
  #endif //ndef IG_INDUSTRY_FACTORY_DETAIL_BASE
#else //BOOST_PP_IS_ITERATING

#define N             BOOST_PP_ITERATION()
#define TYPENAME_Mn   BOOST_PP_ENUM_PARAMS(N,typename M)
#define Mn            BOOST_PP_ENUM_PARAMS(N,M)

namespace industry {
	template < typename KeyT , typename Interface , TYPENAME_Mn >
	class factory_base< KeyT , Interface , methods< Mn > >
		: public crtp_template_inherit< factory_detail::factory_aspect ,
		         factory_base< KeyT , Interface , methods< Mn > > , Mn >
	{
		typedef KeyT                                                                           key_type;
		typedef Interface                                                                      interface_type;
		typedef methods< Mn >                                                                  raw_methods_type;
		typedef typename factory_detail::coerce_methods< Interface , raw_methods_type >::type  methods_type;
		typedef factory_detail::vtable< methods_type >                                         vtable_type;
		template < typename , typename > friend class factory_detail::factory_aspect;
		std::map< key_type , vtable_type * > tables;
	public:
		BOOST_PP_REPEAT(n,INDUSTRY_FACTORY_ASPECT_CREATE_use,N)
		template < typename T > void type( key_type key ) {
			if ( tables[ key ] ) return;
			tables[ key ] = vtable_type::template create< interface_type::template aspect , T >();
		}
	};
	
	template < typename Interface , TYPENAME_Mn >
	class factory_base< industry::type , Interface , methods< Mn > >
		: public crtp_template_inherit< factory_detail::factory_aspect ,
		         factory_base< industry::type , Interface , methods< Mn > > , Mn >
	{
		typedef industry::type                                                                 key_type;
		typedef Interface                                                                      interface_type;
		typedef methods< Mn >                                                                  raw_methods_type;
		typedef typename factory_detail::coerce_methods< Interface , raw_methods_type >::type  methods_type;
		typedef factory_detail::vtable< methods_type >                                         vtable_type;
		template < typename , typename > friend class factory_detail::factory_aspect;
		std::map< key_type , vtable_type * > tables;
	public:
		BOOST_PP_REPEAT(n,INDUSTRY_FACTORY_ASPECT_CREATE_use,N)
		template < typename T > void type( key_type key ) {
			if ( tables[ key ] ) return;
			tables[ key ] = vtable_type::template create< interface_type::template aspect , T >();
		}
		template < typename T > void auto_type() {
			if ( tables[ typeid(T) ] ) return;
			tables[ typeid(T) ] = vtable_type::template create< interface_type::template aspect , T >();
		}
	};
}

#endif //BOOST_PP_IS_ITERATING
