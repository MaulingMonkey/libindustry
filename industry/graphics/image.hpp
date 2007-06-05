// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_GRAPHICS_IMAGE
#define IG_INDUSTRY_GRAPHICS_IMAGE

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4510 ) // STFU about ungenerateable default ctors -- We know about unconstructable concept classes in boost::multi_array
#pragma warning( disable: 4610 ) // STFU about uninstantiateable classes -- We know about unconstructable concept classes in boost::multi_array
#endif

#include <industry/graphics/color.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <algorithm>
#include <memory>

#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace industry {
	namespace graphics {
		template < typename ColorT >
		class image_view {
			struct nouse {};
		public:
			typedef typename get_interface_color_type<ColorT>::type           color_type;
			typedef typename boost::mpl::if_< boost::is_same<ColorT,color_type>, nouse, color_type >::type    extra_color_type;


		protected:
			virtual       ColorT& at( unsigned x, unsigned y )       = 0;
			virtual const ColorT& at( unsigned x, unsigned y ) const = 0;


		public:
			class reference {
				image_view* self;
				unsigned x, y;
			public:
				reference( image_view* self, unsigned x, unsigned y ): self(self), x(x), y(y) {}
				operator ColorT&         () const { return self->at(x,y); }
				operator extra_color_type() const { return self->at(x,y); }
				reference& operator=( const ColorT          & color ) { self->at(x,y) = color; return *this; }
				reference& operator=( const extra_color_type& color ) { self->at(x,y) = color; return *this; }
			};
			class const_reference {
				const image_view* self;
				unsigned x, y;
			public:
				const_reference( const image_view* self, unsigned x, unsigned y ): self(self), x(x), y(y) {}
				operator ColorT          () const { return self->at(x,y); }
				operator extra_color_type() const { return self->at(x,y); }
			};


			virtual unsigned width () const = 0;
			virtual unsigned height() const = 0;

			reference       operator()( unsigned x, unsigned y )       { return reference      (this,x,y); }
			const_reference operator()( unsigned x, unsigned y ) const { return const_reference(this,x,y); }
		};

		template < typename ColorT >
		class image : public image_view<ColorT> {
		public:
			typedef typename image_view<ColorT>::color_type color_type;
		private:
			boost::multi_array<ColorT,2> data_;
			typedef typename boost::multi_array<ColorT,2>::size_type datai;
		protected:
			virtual       ColorT& at( unsigned x, unsigned y )       { return data_[datai(x)][datai(y)]; }
			virtual const ColorT& at( unsigned x, unsigned y ) const { return data_[datai(x)][datai(y)]; }
		public:
			image(): data_() {}
			image( unsigned width, unsigned height ): data_(boost::extents[datai(width)][datai(height)],boost::fortran_storage_order()) {}
			image( const image& other ): data_(other.data) {}

			template < typename OColorT > friend class image;
			template < typename OColorT >
			image( const image_view<OColorT>& other ): data_(boost::extents[datai(other.width())][datai(other.height())]) {
				for ( unsigned y = 0 ; y < other.height() ; ++y ) {
					for ( unsigned x = 0 ; x < other.width() ; ++x ) {
						data_[datai(x)][datai(y)] = color_type(other(x,y));
					}
				}
			}

			virtual unsigned width () const { return unsigned(data_.shape()[0]); }
			virtual unsigned height() const { return unsigned(data_.shape()[1]); }
			void resize( unsigned width, unsigned height ) { data_.resize( boost::extents[datai(width)][datai(height)] ); }

			const size_t data_size() const { return size_t(width())*size_t(height()); }
			      ColorT* data()       { return data_.data(); }
			const ColorT* data() const { return data_.data(); }
		};
	}
}

#endif //ndef IG_INDUSTRY_GRAPHICS_IMAGE
