// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - dot names to directory based name fixes
// Dec  1, 2006 - test.fixed => test.math.fixed
// Jul 13, 2006 - Created

#include <industry/arrays.hpp>
#include <industry/math/fixed.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

template < unsigned n >
void test_fixed_unsigned_limits() {
	typedef industry::fixed< unsigned , n > ufp;
	BOOST_CHECK_EQUAL( ufp::raw( 0xFFFFFFFF ) / ufp(1.0) , ufp::raw( 0xFFFFFFFF ) );
	BOOST_CHECK_EQUAL( ufp::raw( 0xFFFFFFFF ) / ufp::raw(0xFFFFFFFF) , ufp(1.0) );
	BOOST_CHECK_EQUAL( ufp::raw( 0xFFFFFFFE ) / ufp(1.0) , ufp::raw( 0xFFFFFFFE ) );
	BOOST_CHECK_EQUAL( ufp::raw( 0xFFFFFFFE ) / ufp::raw(0xFFFFFFFE) , ufp(1.0) );
}

void test_math_fixed() {
	typedef industry::fixed< int , 6 > fp6;
	typedef industry::fixed< int , 5 > fp5;
	typedef industry::fixed< int , 2 > fp2;
	
	typedef industry::fixed< unsigned , 30 > ufp30;
	
	BOOST_CHECK_CLOSE( fp6( +0.1   ).to_double() , 0.1   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( +0.124 ).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( +0.125 ).to_double() , 0.125 ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( +0.126 ).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( +0.5   ).to_double() , 0.5   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( +0.9   ).to_double() , 0.9   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( +1.0   ).to_double() , 1.0   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( +1.5   ).to_double() , 1.5   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( +2.0   ).to_double() , 2.0   ,  0.001 );

	BOOST_CHECK_CLOSE( fp6( -0.1   ).to_double() , -0.1   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( -0.124 ).to_double() , -0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( -0.125 ).to_double() , -0.125 ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( -0.126 ).to_double() , -0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( -0.5   ).to_double() , -0.5   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( -0.9   ).to_double() , -0.9   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( -1.0   ).to_double() , -1.0   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( -1.5   ).to_double() , -1.5   ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( -2.0   ).to_double() , -2.0   ,  0.001 );
	
	BOOST_CHECK_CLOSE( fp6( "+0.1"   ).to_double() , 0.1   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( "+0.124" ).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( "+0.125" ).to_double() , 0.125 ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( "+0.126" ).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp6( "-0.5"   ).to_double() , -0.5  ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( "-0.9"   ).to_double() , -0.9  , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp6( "-1.0"   ).to_double() , -1.0  ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( "-1.5"   ).to_double() , -1.5  ,  0.001 );
	BOOST_CHECK_CLOSE( fp6( "-2.0"   ).to_double() , -2.0  ,  0.001 );
	
	BOOST_CHECK_CLOSE( fp5(fp6( +0.1   )).to_double() , 0.1   , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp5(fp6( +0.124 )).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp5(fp6( +0.125 )).to_double() , 0.125 ,  0.001 );
	BOOST_CHECK_CLOSE( fp5(fp6( +0.126 )).to_double() , 0.125 ,  0.001 ); //rounding check
	BOOST_CHECK_CLOSE( fp5(fp6( -0.5   )).to_double() , -0.5  ,  0.001 );
	BOOST_CHECK_CLOSE( fp5(fp6( -0.9   )).to_double() , -0.9  , 10     ); //inexactly representable
	BOOST_CHECK_CLOSE( fp5(fp6( -1.0   )).to_double() , -1.0  ,  0.001 );
	BOOST_CHECK_CLOSE( fp5(fp6( -1.5   )).to_double() , -1.5  ,  0.001 );
	BOOST_CHECK_CLOSE( fp5(fp6( -2.0   )).to_double() , -2.0  ,  0.001 );
	
	
	BOOST_CHECK_EQUAL( fp2( 0.75 ) + fp2( 0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL( fp2( 0.75 ) - fp2(-0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL(+fp2( 0.75 ) + fp2( 0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL(+fp2( 0.75 ) - fp2(-0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL(-fp2(-0.75 ) + fp2( 0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL(-fp2(-0.75 ) - fp2(-0.50 ) , fp2( 1.25 ) );
	BOOST_CHECK_EQUAL( fp2( 0.75 ) + fp2(-0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL( fp2( 0.75 ) - fp2( 0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL(+fp2( 0.75 ) + fp2(-0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL(+fp2( 0.75 ) - fp2( 0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL(-fp2(-0.75 ) + fp2(-0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL(-fp2(-0.75 ) - fp2( 0.50 ) , fp2( 0.25 ) );
	BOOST_CHECK_EQUAL( fp2(-0.75 ) + fp2( 0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL( fp2(-0.75 ) - fp2(-0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL(+fp2(-0.75 ) + fp2( 0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL(+fp2(-0.75 ) - fp2(-0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL(-fp2( 0.75 ) + fp2( 0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL(-fp2( 0.75 ) - fp2(-0.50 ) , fp2(-0.25 ) );
	BOOST_CHECK_EQUAL( fp2(-0.75 ) + fp2(-0.50 ) , fp2(-1.25 ) );
	BOOST_CHECK_EQUAL( fp2(-0.75 ) - fp2( 0.50 ) , fp2(-1.25 ) );
	BOOST_CHECK_EQUAL(+fp2(-0.75 ) + fp2(-0.50 ) , fp2(-1.25 ) );
	BOOST_CHECK_EQUAL(+fp2(-0.75 ) - fp2( 0.50 ) , fp2(-1.25 ) );
	BOOST_CHECK_EQUAL(-fp2( 0.75 ) + fp2(-0.50 ) , fp2(-1.25 ) );
	BOOST_CHECK_EQUAL(-fp2( 0.75 ) - fp2( 0.50 ) , fp2(-1.25 ) );
	
	BOOST_CHECK_EQUAL( fp2(  0.5 ) * fp2(  0.5 ) , fp2(  0.25 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) * fp2(  0.5 ) , fp2( -0.25 ) );
	BOOST_CHECK_EQUAL( fp2(  0.5 ) * fp2( -0.5 ) , fp2( -0.25 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) * fp2( -0.5 ) , fp2(  0.25 ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) * fp2(  0.5 ) , fp2(  0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) * fp2(  0.5 ) , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) * fp2( -0.5 ) , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) * fp2( -0.5 ) , fp2(  0.5  ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) *  0.5  , fp2(  0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) *  0.5  , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) * -0.5  , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) * -0.5  , fp2(  0.5  ) );
	BOOST_CHECK_EQUAL( fp2(  1.5 ) * fp2( 1.5 ) , fp2( 2.25 ) );


	BOOST_CHECK_EQUAL( fp2(  0.5 ) / fp2(  0.5 ) , fp2(  1.0 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) / fp2(  0.5 ) , fp2( -1.0 ) );
	BOOST_CHECK_EQUAL( fp2(  0.5 ) / fp2( -0.5 ) , fp2( -1.0 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) / fp2( -0.5 ) , fp2(  1.0 ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) / fp2(  0.5 ) , fp2(  2.0  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) / fp2(  0.5 ) , fp2( -2.0  ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) / fp2( -0.5 ) , fp2( -2.0  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) / fp2( -0.5 ) , fp2(  2.0  ) );
	
	BOOST_CHECK_EQUAL( fp2(  0.5 ) / fp2(  2.0 ) , fp2(  0.25 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) / fp2(  2.0 ) , fp2( -0.25 ) );
	BOOST_CHECK_EQUAL( fp2(  0.5 ) / fp2( -2.0 ) , fp2( -0.25 ) );
	BOOST_CHECK_EQUAL( fp2( -0.5 ) / fp2( -2.0 ) , fp2(  0.25 ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) / fp2(  2.0 ) , fp2(  0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) / fp2(  2.0 ) , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2(  1.0 ) / fp2( -2.0 ) , fp2( -0.5  ) );
	BOOST_CHECK_EQUAL( fp2( -1.0 ) / fp2( -2.0 ) , fp2(  0.5  ) );
	
	BOOST_CHECK_EQUAL( ufp30( 1.5 ) / ufp30( 1.5 ) , ufp30( 1u ) );
	BOOST_CHECK_EQUAL( ufp30( 1.5 ) / ufp30( 1.0 ) , ufp30( 1.5 ) );
	BOOST_CHECK_EQUAL( ufp30( 1.5 ) / ufp30( 0.5 ) , ufp30( 3u ) );


	static const double constants[] = { 0.25 , 0.50 , 0.75 , 1.00 , 2.00 , 3.00 , 4.00 , 4.25 , 4.50 , 4.75
	                                  , 1000 , 1024 , 4949 , 9999 , 9999 , 6666 , 0666 , 8393 , 1284 , 1378
	                                  , 13903859.25 , 183794646.5 , 183794646.75 };
	
	using namespace industry::arrays;
	for ( const double * a = begin( constants ) ; a != end( constants ) ; ++a ) {
		for ( const double * b = begin( constants ) ; b != end( constants ) ; ++b ) {
			if ( *b * double(std::numeric_limits< int >::max())/4.0 < +*a ) continue; //skip - would overflow
			if ( *b * double(std::numeric_limits< int >::min())/4.0 > -*a ) continue; //skip - would underflow
			
			BOOST_CHECK_MESSAGE( fp2(+*a)/fp2(+*b) == fp2::round0(+*a/+*b)
			                   ,"fp2(+*a)/fp2(+*b) == fp2::round0(+*a/+*b) failed for *a==" << *a << " b==" << *b << " ("
			                   << fp2(+*a)/fp2(+*b) << "!=" << fp2::round0(*+a/+*b) << ")"
			                   );
			BOOST_CHECK_MESSAGE( fp2(+*a)/fp2(-*b) == fp2::round0(+*a/-*b)
			                   ,"fp2(+*a)/fp2(-*b) == fp2::round0(+*a/-*b) failed for *a==" << *a << " b==" << *b << " ("
			                   << fp2(+*a)/fp2(-*b) << "!=" << fp2::round0(*+a/-*b) << ")"
			                   );
			BOOST_CHECK_MESSAGE( fp2(-*a)/fp2(+*b) == fp2::round0(-*a/+*b)
			                   ,"fp2(-*a)/fp2(+*b) == fp2::round0(-*a/+*b) failed for *a==" << *a << " b==" << *b << " ("
			                   << fp2(-*a)/fp2(+*b) << "!=" << fp2::round0(-*a/+*b) << ")"
			                   );
			BOOST_CHECK_MESSAGE( fp2(-*a)/fp2(-*b) == fp2::round0(-*a/-*b)
			                   ,"fp2(-*a)/fp2(-*b) == fp2::round0(-*a/-*b) failed for *a==" << *a << " b==" << *b << " ("
			                   << fp2(-*a)/fp2(-*b) << "!=" << fp2::round0(-*a/-*b) << ")"
			                   );
		}
	}
	
	test_fixed_unsigned_limits<0>();
	test_fixed_unsigned_limits<1>();
	test_fixed_unsigned_limits<2>();
	test_fixed_unsigned_limits<3>();
	test_fixed_unsigned_limits<4>();
	test_fixed_unsigned_limits<5>();
	test_fixed_unsigned_limits<6>();
	test_fixed_unsigned_limits<7>();
	test_fixed_unsigned_limits<8>();
	test_fixed_unsigned_limits<9>();
	test_fixed_unsigned_limits<10>();
	test_fixed_unsigned_limits<11>();
	test_fixed_unsigned_limits<12>();
	test_fixed_unsigned_limits<13>();
	test_fixed_unsigned_limits<14>();
	test_fixed_unsigned_limits<15>();
	test_fixed_unsigned_limits<16>();
	test_fixed_unsigned_limits<17>();
	test_fixed_unsigned_limits<18>();
	test_fixed_unsigned_limits<19>();
	test_fixed_unsigned_limits<20>();
	test_fixed_unsigned_limits<21>();
	test_fixed_unsigned_limits<22>();
	test_fixed_unsigned_limits<23>();
	test_fixed_unsigned_limits<24>();
	test_fixed_unsigned_limits<25>();
	test_fixed_unsigned_limits<26>();
	test_fixed_unsigned_limits<27>();
	test_fixed_unsigned_limits<28>();
	test_fixed_unsigned_limits<29>();
	test_fixed_unsigned_limits<30>();
	test_fixed_unsigned_limits<31>();
	//test_fixed_unsigned_limits<32>();
	
	BOOST_CHECK_EQUAL( fp6( +0.124 ) , fp6( +0.126 ) );
	BOOST_CHECK( !( fp6( +0.124 ) == fp6(-0.126) ) );
	BOOST_CHECK(  ( fp6( +0.124 ) != fp6(-0.126) ) );
	BOOST_CHECK_EQUAL( boost::lexical_cast< fp6 >( "-0.124" ) , fp6( -0.124 ) );
	
	BOOST_CHECK_EQUAL( (industry::fixed<unsigned,2>::raw(1<<2)) , (industry::fixed<short,4>::raw(1<<4)) );
	BOOST_CHECK      ( (industry::fixed<unsigned,2>::raw(1<<2)) != (industry::fixed<short,4>::raw((1<<4)+1)) );
	
	industry::fixed< int , 2 >::raw(1<<2).to_integer();
	
	//these don't compile:
	//industry::fixed< unsigned , 33 > shouldnt_compile_1;
	//industry::fixed< int , 32 > shouldnt_compile_2;
	
	//industry::fixed< unsigned , 32 > should_compile_1; //--- does, causes annoying warning --- fixme?
	industry::fixed< int , 31 > should_compile_2;
}
