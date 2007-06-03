// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_GRAPHICS_COLOR
#define IG_INDUSTRY_GRAPHICS_COLOR

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4800 ) //STFU about uint -> bool
#endif //def _MSC_VER

#include <boost/static_assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor.hpp>
#include <boost/array.hpp>
#include <climits>

namespace industry {
	namespace graphics {
		template < template < typename > class ColorT > struct packed565 {
			typedef ColorT<unsigned char> interface_color_type;

			unsigned short data;

			static const size_t         lshift0=11    , lshift1= 5    , lshift2= 0    ;
			static const size_t         cshift0= 3    , cshift1= 2    , cshift2= 3    ;
			static const size_t         bits0  = 5    , bits1  = 6    , bits2  = 5    ;
			static const unsigned short mask0=(1<<bits0)-1, mask1=(1<<bits1)-1, mask2=(1<<bits2)-1;

			packed565(): data() {}
			packed565( unsigned char c1, unsigned char c2, unsigned char c3 ) {
				*this = ColorT<unsigned char>(c1,c2,c3);
			}
			packed565( const ColorT< unsigned char > & c ) {
				data  = (((c[0]>>cshift0)&mask0)<<lshift0)
				      + (((c[1]>>cshift1)&mask1)<<lshift1)
				      + (((c[2]>>cshift2)&mask2)<<lshift2)
				      ;
			}

			operator ColorT<unsigned char>() const {
				return ColorT<unsigned char>( (unsigned char)((data>>lshift0)&mask0) << cshift0
				                            , (unsigned char)((data>>lshift1)&mask1) << cshift1
				                            , (unsigned char)((data>>lshift2)&mask2) << cshift2
				                            );
			}
		};
		template < typename T > struct rgba;
		template < typename T > struct greyscale {
			T grey;

			greyscale(): grey() {}
			greyscale( T grey ): grey(grey) {}

			      T& operator[]( unsigned i )       { assert(i==0); return grey; }
			const T& operator[]( unsigned i ) const { assert(i==0); return grey; }
		private:
			T max() { return T(boost::is_same<T,unsigned char>::value ? 1 : 255); }
		};
		template < typename T > struct rgb {
			T red, green, blue;

			rgb(): red(), green(), blue() {}
			rgb( const rgba<T>& o ): red(o.red), green(o.green), blue(o.blue) {}
			rgb( T red, T green, T blue ): red(red), green(green), blue(blue) {}

			      T& operator[]( unsigned i )       { switch (i) { case 0: return red; case 1: return green; case 2: return blue; default: assert(0); } }
			const T& operator[]( unsigned i ) const { switch (i) { case 0: return red; case 1: return green; case 2: return blue; default: assert(0); } }
		private:
			T max() { return T(boost::is_same<T,unsigned char>::value ? 1 : 255); }
		};
		template < typename T > struct rgba {
			T red, green, blue, alpha;

			rgba(): red(), green(), blue(), alpha() {}
			rgba( const rgb<T>& o ): red(o.red), green(o.green), blue(o.blue), alpha(max()) {}
			rgba( T red, T green, T blue, T alpha ): red(red), green(green), blue(blue), alpha(alpha) {}

			      T& operator[]( unsigned i )       { switch (i) { case 0: return red; case 1: return green; case 2: return blue; case 3: return alpha; default: assert(0); } }
			const T& operator[]( unsigned i ) const { switch (i) { case 0: return red; case 1: return green; case 2: return blue; case 3: return alpha; default: assert(0); } }
		private:
			T max() { return T(boost::is_same<T,unsigned char>::value ? 1 : 255); }
		};

		BOOST_MPL_HAS_XXX_TRAIT_DEF( interface_color_type );
		template < typename T > struct get_interface_color_type {
		private:
			template < typename T > struct do_get { typedef typename T::interface_color_type type; };
		public:
			typedef typename boost::mpl::eval_if< has_interface_color_type<T>, do_get<T>, boost::mpl::identity<T> >::type type;
		};
	}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif //def _MSC_VER

#endif //ndef IG_INDUSTRY_GRAPHICS_COLOR
