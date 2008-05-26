// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/languages/ruby/detail/registries.hpp>

namespace industry { namespace languages { namespace ruby { namespace detail {
	std::map<void*,VALUE> instances;
}}}}
