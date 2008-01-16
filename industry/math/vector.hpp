// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - industry.math.vector.hpp => industry/math/vector.hpp
// Dec  1, 2006 - Created

#ifndef BOOST_PP_IS_ITERATING
	#ifndef IG_INDUSTRY_MATH_VECTOR
	#define IG_INDUSTRY_MATH_VECTOR

	#ifndef INDUSTRY_MATH_VECTOR_DN_LIMIT
	#define INDUSTRY_MATH_VECTOR_DN_LIMIT 3
	#endif //ndef  INDUSTRY_MATH_VECTOR_DN_LIMIT

	#include <boost/preprocessor/iteration.hpp>
	#include <boost/preprocessor/repetition.hpp>
	#include <boost/static_assert.hpp>
	#include <cassert>
	#include <cmath>
	#include <numeric>

namespace industry {
	namespace math {
		template < typename T , unsigned Dn >
		class vector;
		
		template < typename T , unsigned Dn >
		struct vector_base;
		
		template < typename T >
		struct vector_base< T , 0 >; //no implementation
		
		template < typename T >
		struct vector_base< T , 1 > {
			T x;
			      T & operator[]( unsigned n )       { assert(n<1); return x; }
			const T & operator[]( unsigned n ) const { assert(n<1); return x; }
		};
		template < typename T >
		struct vector_base< T , 2 > {
			T x,y;
			      T & operator[]( unsigned n )       { assert(n<2); return *(&x+n); }
			const T & operator[]( unsigned n ) const { assert(n<2); return *(&x+n); }
		};
		template < typename T >
		struct vector_base< T , 3 > {
			T x,y,z;
			      T & operator[]( unsigned n )       { assert(n<3); return *(&x+n); }
			const T & operator[]( unsigned n ) const { assert(n<3); return *(&x+n); }
		};
		template < typename T , unsigned Dn >
		struct vector_base {
			BOOST_STATIC_ASSERT(( Dn > 3 ));
			T x,y,z;
		protected:
			T etc[ Dn-3 ];
			      T & operator[]( unsigned n )       { assert(n<Dn); return (n<=3) ? *(&x+n) : (etc[n-3]); }
			const T & operator[]( unsigned n ) const { assert(n<Dn); return (n<=3) ? *(&x+n) : (etc[n-3]); }
		};
		
		#define BOOST_PP_ITERATION_LIMITS (1,INDUSTRY_MATH_VECTOR_DN_LIMIT)
		#define BOOST_PP_FILENAME_1 <industry/math/vector.hpp>
		#include BOOST_PP_ITERATE()
		
		template < typename T , unsigned N >
		bool operator==( const vector<T,N> & lhs , const vector<T,N> & rhs ) {
			for ( unsigned i = 0 ; i < N ; ++i ) {
				if ( lhs[i] != rhs[i] ) return false;
			}
			return true;
		}
		template < typename T , unsigned N >
		bool operator!=( const vector<T,N> & lhs , const vector<T,N> & rhs ) {
			return !(lhs==rhs);
		}
	}
}
	
	#endif //ndef IG_INDUSTRY_MATH_VECTOR
	
#else //BOOST_PP_IS_ITERATING
	#define DN                                             BOOST_PP_ITERATION()
	#define INDUSTRY_MATH_VECTOR_FILL_THIS_ARGS(z,n,src)   (*this)[n]  = src ## n;

template < typename T >
class vector< T , DN > : public vector_base< T , DN > {
public:
	vector() { std::fill_n(&(*this)[0], DN, T()); }
	vector( BOOST_PP_ENUM_PARAMS(DN,T elem) ) { BOOST_PP_REPEAT(DN,INDUSTRY_MATH_VECTOR_FILL_THIS_ARGS,elem); }
	
	vector& operator+=( const vector & other ) { for ( unsigned i = 0 ; i < DN ; ++i ) (*this)[i] += other[i]; return *this; }
	vector& operator-=( const vector & other ) { for ( unsigned i = 0 ; i < DN ; ++i ) (*this)[i] -= other[i]; return *this; }
	vector& operator*=( const T &      value ) { for ( unsigned i = 0 ; i < DN ; ++i ) (*this)[i] *= value; return *this; }
	vector& operator/=( const T &      value ) { for ( unsigned i = 0 ; i < DN ; ++i ) (*this)[i] /= value; return *this; }
	
	friend vector operator+( const vector & lhs , const vector & rhs ) { vector c(lhs); c += rhs; return c; }
	friend vector operator-( const vector & lhs , const vector & rhs ) { vector c(lhs); c -= rhs; return c; }
	friend vector operator*( const vector & lhs , const T      & rhs ) { vector c(lhs); c *= rhs; return c; }
	friend vector operator/( const vector & lhs , const T      & rhs ) { vector c(lhs); c /= rhs; return c; }
	
	friend T square_magnitude( const vector & v ) { return std::inner_product(&v[0], &v[0] + DN, &v[0], T()); }
	friend T magnitude ( const vector & v ) { return T( std::sqrt( square_magnitude( v ) ) ); }
	
	template < typename U > friend U square_magnitude( const vector & v ) { return static_cast<U>(square_magnitude(v)); }
	template < typename U > friend U magnitude ( const vector & v ) { return U( std::sqrt( square_magnitude/*<U>*/(v) ) ); }
};


	#undef DN
	#undef INDUSTRY_MATH_VECTOR_FILL_THIS_ARGS
#endif //BOOST_PP_IS_ITERATING
