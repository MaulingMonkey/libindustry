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

	std::map<VALUE,VALUE>& remap_registry() {
		static bool initialized = false;
		static std::map<VALUE,VALUE> r;

		if (!initialized) {

#define RUBY_T_REMAP( origin, equivalent ) r[origin]=equivalent;
#include <industry/languages/ruby/detail/builtins.hpp>
#undef RUBY_T_REMAP

			initialized = true;
		}
		return r;
	}
}}}}
