// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/pod/interleaved_source.hpp>
#include <industry/pod/tuple.hpp>
#include <industry/traits/pod.hpp>
#include <boost/test/unit_test.hpp>

namespace {
	struct pod1 { int a, b, c; };

	struct nonpod1 {
		typedef pod1 pod_type;

		int a, b, c;
		nonpod1( int a, int b, int c ): a(a), b(b), c(c) {}
		nonpod1( const pod_type& p ): a(p.a), b(p.b), c(p.c) {}
		
		operator pod_type() const { pod_type p = {a,b,c}; return p; }
	};

	struct nonpod2 {
		int a, b, c;
		nonpod2( int a, int b, int c ): a(a), b(b), c(c) {}
	};
}

namespace industry {
	namespace pod {
		template<> struct to_pod_conversion_traits< nonpod2 > {
			typedef pod1 pod_type;
			static pod_type convert( const nonpod2& n ) { pod_type p = {n.a, n.b, n.c}; return p; }
		};
		template<> struct from_pod_conversion_traits< nonpod2 , pod1 > {
			static nonpod2 convert( const pod1& p ) { return nonpod2(p.a,p.b,p.c); }
		};
	}
}

void test_pod( void ) {
	pod1    legal1 = {1, 2, 3}; //OK
	//nonpod1 illegal1 = {1, 2, 3}; //OK (error)
	//nonpod2 illegal2 = {1, 2, 3}; //OK (error)

	industry::pod::interleaved_source< pod1, nonpod1, nonpod2 > data[] = {
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	};
}
