// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_SELF_AWARE_BASE
#define IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_SELF_AWARE_BASE

#include <industry/languages/ruby/detail/intrusive.hpp>
#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>

namespace industry { namespace languages { namespace ruby { namespace detail {
	template < typename T > struct class_registry;
	template < typename T > struct instance_registry;

	template < typename T > class self_aware_base {
		VALUE v;
		friend struct instance_registry<T>;
	public:
		self_aware_base(): v(0) {}
		self_aware_base(const self_aware_base&): v(0) {}

		VALUE raw_self() {
			if (!v) v = Data_Wrap_Struct(class_registry<T>::get(),0,intrusive_gc_and_release_or_noop::call<T>,this); // Starts out owned by C++
			// The above should be the only RData instance associated with this.
			// RDATA(v)->dfree may be modified later.
			return v;
		}
	};
}}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_SELF_AWARE_BASE
