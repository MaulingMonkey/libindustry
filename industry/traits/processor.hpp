// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_TRAITS_PROCESSOR
#define IG_INDUSTRY_TRAITS_PROCESSOR

#include <boost/type_traits/remove_const.hpp>

namespace industry {
	namespace algorithm {
		template < typename Processor > struct coerce_to_processor {
			typedef typename boost::remove_const< Processor >::type type;
		};

		struct unbound_processor_tag {};
		struct   bound_processor_tag {};
		struct  active_processor_tag {};
	}
}

#endif //ndef IG_INDUSTRY_TRAITS_PROCESSOR
