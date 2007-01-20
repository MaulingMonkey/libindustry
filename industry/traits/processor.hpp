// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_TRAITS_PROCESSOR
#define IG_INDUSTRY_TRAITS_PROCESSOR

#include <industry/sfinae.hpp>
#include <industry/traits/range.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace industry {
	struct unbound_processor_tag {};
	struct   bound_processor_tag {};


	namespace detail {
		template < typename Processor > sfinae::one has_processor_category_helper( typename Processor::processor_category* );
		template < typename Processor > sfinae::two has_processor_category_helper( ... );
		template < typename Processor > struct has_processor_category { static const bool value = (sizeof(sfinae::one) == sizeof(has_processor_category_helper<Processor>(0)) ); };

		template < typename Processor > struct get_processor_category_helper { typedef typename Processor::processor_category type; };
		template < typename Processor > struct get_processor_category { typedef typename boost::mpl::eval_if_c< has_processor_category< Processor >::value , get_processor_category_helper< Processor > , boost::mpl::identity< bound_processor_tag > >::type type; };

		template < typename Category , typename Processor > struct processor_traits_default_iterators {};
		template < typename Processor > struct processor_traits_default_iterators< bound_processor_tag , Processor > { typedef typename range_traits< Processor >::iterator iterator; };

		template < typename Category , typename Processor > struct processor_traits_default_after {};
		template < typename Processor > struct processor_traits_default_after< unbound_processor_tag , Processor > { template < typename Preceeding > struct after { typedef typename Processor::template after< Preceeding >::type type; }; };
	}

	template < typename Processor > struct processor_traits
		: detail::processor_traits_default_iterators< typename detail::get_processor_category< Processor >::type , Processor >
		, detail::processor_traits_default_after    < typename detail::get_processor_category< Processor >::type , Processor >
	{
		typedef typename detail::get_processor_category< Processor >::type processor_category;
	};
}

#endif //ndef IG_INDUSTRY_TRAITS_PROCESSOR
