// Copyright (c) 2006 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
// Adopted changes from deffer (GDNet) 

#include <iostream>

#include <industry/algorithm.hpp>
#include <industry/arrays.hpp>
#include <industry/range.hpp>
#include <industry/math/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <iterator>
#include <vector>
#include <boost/bind.hpp>

namespace {
	inline bool is_odd( unsigned value )  { return  (value & 0x1); }
	inline bool is_even( unsigned value ) { return !is_odd(value); }
}

void test_algorithm( void ) {
	using namespace industry;

	unsigned data[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 };
	unsigned expected_results_odd[] = { 1 , 3 , 5 , 7 };
	unsigned expected_results_even[] = { 2, 4, 6, 8 };

	const std::vector< unsigned > data2( begin(data) , end(data) );
	std::vector< unsigned > results;
	
	data | filter( is_odd ) | push_back(results);

	BOOST_CHECK(( make_range(results) == make_range(expected_results_odd) ));

	results.clear();

	data2 | filter( is_even ) | push_back( results );

	BOOST_CHECK(( make_range(expected_results_even) == make_range(results) ));
}

namespace {
	struct base {
		base() : point() {}
		base(int p) : point(p) {}
		void add_point() { ++point; }
		int point;
	};

	struct child : base {
		child() {}
		child(int p) : base(p) {}
		base& get_parent() { return *static_cast<base*>(this); }
		base* get_parent_ptr() { return static_cast<base*>(this); }
		bool operator==(child const& other) {
			return point == other.point;
		}
	};

	void print_point(child& p) {
		BOOST_CHECK(p.point == 1);
	}

	void print_point(child& p, int i) {
		BOOST_CHECK(p.point == 1);
	}
	
	void print_point_ptr(base* p) {
		BOOST_CHECK(p->point == 1);
	}

	void initialize_vector(industry::math::vector<float, 3>& v) {
		v[0] = 1.0f;
		v[1] = 2.0f;
		v[2] = 3.0f;
	}
}

void test_algorithm_call() {
	using namespace industry;

	child data[10];
	std::vector<child> expected_result(10, child(1));

	data | call(&child::add_point);
	data | call(print_point);
	data | call<void, child&>(boost::bind(print_point, _1, 2));

	BOOST_CHECK(( make_range(expected_result) == make_range(data) ));

	std::vector< math::vector<float, 3> > particles(10);
	particles | call(initialize_vector);

	std::vector< math::vector<float, 3> > expected_particles_result(10, math::vector<float, 3>(1.0f, 2.0f, 3.0f));

	BOOST_CHECK(( make_range(expected_particles_result) == make_range(particles) ));
}

void test_algorithm_transform() {
	using namespace industry;

	child data[10];
	std::vector<child> expected_result(10, child(1));

	data | transform(&child::get_parent) | call(&base::add_point);
	data | transform(&child::get_parent_ptr) | call(print_point_ptr);
	data | call(print_point);
	data | call<void, child&>(boost::bind(print_point, _1, 2));

	BOOST_CHECK(( make_range(expected_result) == make_range(data) ));
}