// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 25, 2006 - industry.math.fixed.hpp => industry/math/fixed.hpp
// Dec  1, 2006 - industry.fixed => industry.math.fixed
// Nov 30, 2006 - Did a lot of work, finally got div working - renabled.
// Nov 29, 2006 - Renabled header, multiplication/division disabled.
// Jul 21, 2006 - Got stumped on fixed point division.  Disabling the header.
// Jul 13, 2006 - Created

#ifndef IG_INDUSTRY_MATH_FIXED
#define IG_INDUSTRY_MATH_FIXED

#define INDUSTRY_FIXED_DIV_TRACE(x)// std::cout << x << std::endl

#include <industry/math/utility.hpp>
#include <boost/mpl/if.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <cassert>

namespace industry {
	namespace math {
		template < typename StorageT , unsigned offset > class fixed;
	
		template < typename StorageT , unsigned offset >
		class fixed {
			template < typename , unsigned > friend class fixed;
			
			static const unsigned offset_limit = sizeof(StorageT) * CHAR_BIT - ( std::numeric_limits< StorageT >::is_signed ? 1 : 0 );
			BOOST_STATIC_ASSERT( offset_limit >= offset );
			BOOST_STATIC_ASSERT( std::numeric_limits< StorageT >::is_integer );
			
			typedef StorageT storage_type;
			storage_type data;
		public:
			static fixed raw( storage_type bits ) {
				fixed r; r.data = bits; return r;
			}
			static fixed round0( float src ) {
				return raw( src * (1u << offset) );
			}
			static fixed round0( double src ) {
				return raw( storage_type( src * (1u << offset) ) );
			}
			static fixed round0( const char * src ) {
				return round0( boost::lexical_cast< double >( src ) );
			}
			fixed() : data() {}
			explicit fixed( storage_type src ) {
				assert( std::numeric_limits< storage_type >::max() / (1<<offset) >= src );
				data = src * (1 << offset);
			}
			explicit fixed( float src ) {
				assert( std::numeric_limits< storage_type >::max() / (1<<offset) >= src );
				data = storage_type( src * (1u << offset) + (src > 0 ? +0.5f : -0.5f) );
			}
			explicit fixed( double src ) {
				assert( std::numeric_limits< storage_type >::max() / (1<<offset) >= src );
				data = storage_type( src * (1u << offset) + (src > 0 ? +0.5 : -0.5) );
			}
			explicit fixed( const char * src ) {
				(*this) = fixed( boost::lexical_cast< double >( src ) );
			}
			
			template < typename OST , unsigned OO > friend class fixed;
			
			template < typename OST , unsigned OO >
			explicit fixed( const fixed< OST , OO > & other ) {
				const unsigned offset_d = offset >= OO ? (offset - OO) : (OO - offset);
				data = storage_type( offset >= OO ? (other.data * (1 << offset_d)) : (other.data / (1 << offset_d)) );
			}
			
			double to_double() const {
				return double(data) / (1u << offset);
			}
			float to_float() const {
				return float(data) / (1u << offset);
			}
			storage_type to_integer() const {
				return data / storage_type(1u << offset);
			}
			storage_type to_raw_data() const {
				return data;
			}
			static const storage_type multiple = (offset<offset_limit) ? (1u<<offset) : 0;
			
			fixed operator-() const {
				return raw( -data );
			}
			fixed operator+() const {
				return *this;
			}
					
			template < typename RST , unsigned RO >
			friend bool operator==( const fixed & lhs , const fixed< RST , RO > & rhs ) {
				typedef StorageT LST;
				const unsigned LO = offset;
				
				//reject sign incompatibilities:
				if ( is_positive(lhs.data) != is_positive(rhs.to_raw_data()) ) return false;
								
				//common use type:
				typedef typename boost::mpl::if_c<
					sizeof(LST) == sizeof(RST)
						?   !std::numeric_limits<LST>::is_signed //prefer unsigned (to avoid overflow -- numbers are >= 0)
						:   (sizeof(LST) > sizeof(RST))          //prefer biggest if differing sizes though
					, /* then */ LST
					, /* else */ RST
					>::type T;
				
				const unsigned DO = (LO > RO) ? (LO - RO) : (RO - LO); //difference in offset
				const T T_max = std::numeric_limits< T >::max();
				const T l = T(lhs.to_raw_data());
				const T r = T(rhs.to_raw_data());
				
				const T l_hi = ( (LO<RO) ? (l) : (l/(1<<DO)) ); //spills over into part of lo (not a problem)
				const T r_hi = ( (LO>RO) ? (r) : (r/(1<<DO)) );
				
				//reject T overflows (since the other is representable in terms of T, an overflow is greater than this):
				//reason: signed overflows are undefined (?) rather than truncating.
				if ( (LO<RO) && T_max / (1<<DO) < l ) return false;
				if ( (LO>RO) && T_max / (1<<DO) < r ) return false;
				
				const T l_lo = ( (LO<RO) ? (l*(1<<DO)) : (l) );
				const T r_lo = ( (LO>RO) ? (r*(1<<DO)) : (r) );
				
				return ((l_lo == r_lo) && (l_hi == r_hi));
			}
			
			friend bool operator<(const fixed& left, const fixed& right) {
				return left.data < right.data;
			}

			friend bool operator>(const fixed& left, const fixed& right) {
				return left.data < right.data;
			}

			friend bool operator<=(const fixed& left, const fixed& right) {
				return left.data <= right.data;
			}

			friend bool operator>=(const fixed& left, const fixed& right) {
				return left.data >= right.data;
			}

			friend std::ostream & operator<<( std::ostream & os , const fixed & f ) {
				os << f.to_double();
				return os;
			}
			friend std::istream & operator>>( std::istream & is , fixed & f ) {
				double src;
				is >> src;
				f = fixed(src);
				return is;
			}
			
			fixed & operator+=( const fixed & other ) {
				data += other.data;
				return *this;
			}
			fixed & operator-=( const fixed & other ) {
				data -= other.data;
				return *this;
			}
			friend fixed operator+( const fixed & lhs , const fixed & rhs ) {
				fixed v = lhs;
				v += rhs;
				return v;
			}
			friend fixed operator-( const fixed & lhs , const fixed & rhs ) {
				fixed v = lhs;
				v -= rhs;
				return v;
			}		
			fixed & operator*=( double other ) {
				data = storage_type(data * other);
				return *this;
			}
			fixed & operator*=( storage_type other ) {
				data *= other;
				return *this;
			}
			fixed & operator*=( const fixed & other ) {
				data
					= ( (data / storage_type(1u<<offset)) * (other.data / storage_type(1u<<offset)) ) * storage_type(1u<<offset)
					+ ( (data % storage_type(1u<<offset)) * (other.data / storage_type(1u<<offset)) )
					+ ( (data / storage_type(1u<<offset)) * (other.data % storage_type(1u<<offset)) )
					+ ( (data % storage_type(1u<<offset)) * (other.data % storage_type(1u<<offset)) ) / storage_type(1u<<offset)
					;
				return *this;
			}
			
			fixed & operator/=( double other ) {
				data = storage_type(data / other);
				return *this;
			}
			fixed & operator/=( storage_type other ) {
				data /= other;
				return *this;
			}
			fixed & operator/=( const fixed & other ) {
				//TODO: Sanity check this.  Yet again.  Hell, just sanity check it forever.
				
				storage_type value = 0;
				storage_type numer = data;       //const bool numer_pos = numer >= 0;
				storage_type denom = other.data; //const bool denom_pos = denom >= 0;
				unsigned remaining_shift = offset;
				INDUSTRY_FIXED_DIV_TRACE( "calculation: " << numer << " / " << denom << " << " << remaining_shift );
	
				const storage_type storage_max = std::numeric_limits< storage_type >::max();
				const storage_type storage_min = std::numeric_limits< storage_type >::min();
				
				//1. Factor out as many 2s from denominator as possible
				while ( remaining_shift && !(denom%2) ) denom /= 2 , --remaining_shift;
				INDUSTRY_FIXED_DIV_TRACE( "[1 ] => " << value << " + " << numer << " / " << denom << " << " << remaining_shift );
				
				//2. Deal with the (remaining) numerator
				do {
					//2a. Multiply as many remaining 2s as possibe (without overflow)
					if (is_positive(numer)) while ( remaining_shift && numer < storage_max/2 ) numer *= 2 , --remaining_shift;
					else                    while ( remaining_shift && numer > storage_min/2 ) numer *= 2 , --remaining_shift;
					INDUSTRY_FIXED_DIV_TRACE( "[2a] => " << value << " + " << numer << " / " << denom << " << " << remaining_shift );
					
					//2b. Over/underflow rejection
					const storage_type remaining = (is_positive(numer) == is_positive(denom))
					                             ? /*then*/ (storage_max-value)
					                             : /*else*/ (storage_min-value)
					                             ;
					if (is_positive(numer) == is_positive(denom)) {
						assert( storage_max / (1<<remaining_shift) >= numer/denom && "overflow" ); //quotient alone overflows
						assert( remaining >= numer/denom*(1<<remaining_shift)     && "overflow" ); //quotient + remaining overflows
					}
					else {
						assert( storage_min / (1<<remaining_shift) <= numer/denom && "underflow" ); //quotient alone underflows
						assert( remaining <= numer/denom*(1<<remaining_shift)     && "underflow" ); //quotient + remaining underflows
					}
					
					//2c. If we have (abs(numer/denom) < 1) and are unable to shift either operand,
					//    forcibly do so (round down) to avoid deadlock
					const bool inadequate_ratio = (is_positive(numer)==is_positive(denom))
					                            ? (numer/denom<storage_type(+1))
					                            : (numer/denom>storage_type(-1))
					                            ;
					const bool numer_pos_locked = (is_positive(numer)==is_positive(denom))
					                            ? (storage_max/2>=numer)
					                            : (storage_min/2<=numer)
					                            ;
					const bool denom_pos_locked = (denom%2) == 1;
					
					if ( remaining_shift && inadequate_ratio && numer_pos_locked && denom_pos_locked ) {
						if (is_positive(denom)) denom = (denom+1)/2;
						else                    denom = (denom-1)/2;
						--remaining_shift;
						INDUSTRY_FIXED_DIV_TRACE( "[2c] => " << value << " + " << numer << " / " << denom << " << " << remaining_shift );
					}
					
					value += numer/denom * (1<<remaining_shift); //2d[.1]  Calculate numerator addition
					numer %= denom;                              //2d[.2]  Remaining numerator for next calc
					INDUSTRY_FIXED_DIV_TRACE( "[2d] => " << value << " + " << numer << " / " << denom << " << " << remaining_shift );
				} while ( remaining_shift && numer );
				
				//3. Finish
				data = value;
				return *this;
			}
			friend fixed operator*( const fixed & lhs , storage_type rhs ) {
				fixed v = lhs;
				v *= rhs;
				return v;
			}
			friend fixed operator*( storage_type lhs , const fixed & rhs ) {
				fixed v = rhs;
				v *= lhs;
				return v;
			}
			friend fixed operator*( const fixed & lhs , double rhs ) {
				fixed v = lhs;
				v *= rhs;
				return v;
			}
			friend fixed operator*( double lhs , const fixed & rhs ) {
				fixed v = rhs;
				v *= lhs;
				return v;
			}
			friend fixed operator*( const fixed & lhs , const fixed & rhs ) {
				fixed v = lhs;
				v *= rhs;
				return v;
			}
			
			friend fixed operator/( const fixed & lhs , storage_type rhs ) {
				fixed v = lhs;
				v /= rhs;
				return v;
			}
			friend fixed operator/( storage_type lhs , const fixed & rhs ) {
				fixed v = rhs;
				v /= lhs;
				return v;
			}
			friend fixed operator/( const fixed & lhs , double rhs ) {
				fixed v = lhs;
				v /= rhs;
				return v;
			}
			friend fixed operator/( double lhs , const fixed & rhs ) {
				fixed v = rhs;
				v /= lhs;
				return v;
			}
			friend fixed operator/( const fixed & lhs , const fixed & rhs ) {
				fixed v = lhs;
				v /= rhs;
				return v;
			}
		};
		
		template < typename LST , typename RST , unsigned LO , unsigned HO >
		bool operator!=( const fixed< LST , LO > & lhs , const fixed< RST , HO > & rhs ) {
			return !(lhs == rhs);
		}
	}
	using namespace math;
}

#endif //ndef IG_INDUSTRY_MATH_FIXED
