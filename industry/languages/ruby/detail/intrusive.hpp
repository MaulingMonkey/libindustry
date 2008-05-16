// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_INTRUSIVE
#define IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_INTRUSIVE

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <boost/intrusive_ptr.hpp>

namespace industry { namespace languages { namespace ruby { namespace detail {
		namespace intrusive_add_ref_or_noop {
			using namespace boost; // pull in definitions from there that work around ADL deficiencies
			template < typename T > void intrusive_ptr_add_ref(T* ptr) {}
			template < typename T > void call( T* ptr ) { intrusive_ptr_add_ref(ptr); } // use this to call ..._add_ref to allow ADL
		}

		namespace intrusive_gc_and_release_or_delete {
			using namespace boost; // pull in definitions from there that work around ADL deficiencies
			template < typename T > void intrusive_ptr_release(T* ptr) { detail::instance_registry<T>::ruby_gced(ptr); delete ptr; }
			template < typename T > void call( T* ptr ) { detail::instance_registry<T>::ruby_gced(ptr); intrusive_ptr_release(ptr); } // use this to call ..._release to allow ADL
		}

		namespace intrusive_gc_and_release_or_noop {
			using namespace boost; // pull in definitions from there that work around ADL deficiencies
			template < typename T > void intrusive_ptr_release(T* ptr) { detail::instance_registry<T>::ruby_gced(ptr); }
			template < typename T > void call( T* ptr ) { detail::instance_registry<T>::ruby_gced(ptr); intrusive_ptr_release(ptr); } // use this to call ..._release to allow ADL
		}
}}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_DETAIL_INTRUSIVE
