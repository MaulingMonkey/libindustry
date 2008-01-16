// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - dot names to directory based name fixes
// Dec  1, 2006 - Created

#define INDUSTRY_MATH_VECTOR_DN_LIMIT 6

#include <industry/math/vector.hpp>
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_math_vector ) {
	using namespace industry::math;
	//vector< unsigned , 0 > should_fail_to_compile_1; //mm: tested 12/1/06 -- OK
	
	vector< unsigned , 1 > should_compile_1;
	vector< unsigned , 2 > should_compile_2;
	vector< unsigned , 3 > should_compile_3;
	vector< unsigned , 4 > should_compile_4;
	vector< unsigned , 5 > should_compile_5;
	vector< unsigned , 6 > should_compile_6;

	typedef vector< float, 3> vf3;
	vf3 v;
	BOOST_CHECK(square_magnitude<double>(v) == 0);
}
