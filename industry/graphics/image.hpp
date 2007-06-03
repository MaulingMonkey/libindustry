// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_GRAPHICS_IMAGE
#define IG_INDUSTRY_GRAPHICS_IMAGE

#include <industry/graphics/image.hpp>
#include <boost/multi_array.hpp>
#include <algorithm>
#include <memory>

namespace industry {
	namespace graphics {
		template < typename ColorT >
		class image {
		public:
			typedef typename get_interface_color_type<ColorT>::type           color_type;
		private:
			typedef          boost::multi_array<ColorT,2>                     data_type;
			typedef typename boost::multi_array<ColorT,2>::size_type          index_type;
			data_type  data;


		public:
			class reference {
				image* self;
				index_type row, column;
			public:
				reference( image* self, index_type row, index_type column ): self(self), row(row), column(column) {}
				operator color_type() const { return self->data[column][row]; }
				reference& operator=( const color_type& color ) { self->data[column][row] = color; return *this; }
			};
			class const_reference {
				const image* self;
				index_type row, column;
			public:
				const_reference( const image* self, index_type row, index_type column ): self(self), row(row), column(column) {}
				operator color_type() const { return self->data[column][row]; }
			};
			template < typename ImageT, typename ReferenceT >
			class slice {
				ImageT* self;
				index_type  column;
			public:
				slice( ImageT* self, index_type  column ): self(self), column(column) {}
				ReferenceT operator[]( index_type  row ) const { return ReferenceT(self,row,column); }
			};


		public:
			image(): data() {}
			image( index_type width, index_type height ): data(boost::extents[width][height]) {}
			image( std::auto_ptr< boost::multi_array<ColorT,2> > stolen_data ) {
				data.swap(stolen_data); // TODO:  Test
			}
			image( std::auto_ptr< image > stolen_image ) {
				data.swap(stolen_image->data); // TODO:  Test
			}
			image( const image& other ): data(other.data) {}

			template < typename OColorT > friend class image;
			template < typename OColorT >
			image( const image<OColorT>& other ): data(boost::extents[other.width()][other.height()]) {
				for ( unsigned y = 0 ; y < other.height() ; ++y ) {
					for ( unsigned x = 0 ; x < other.width() ; ++x ) {
						data[x][y] = color_type( other[x][y] );
					}
				}
			}


			slice< image      , reference       > operator[]( index_type  column )       { return slice< image      , reference       >(this,column); }
			slice< const image, const_reference > operator[]( index_type  column ) const { return slice< const image, const_reference >(this,column); }
			index_type width() const { return data.shape()[0]; }
			index_type height() const { return data.shape()[1]; }
			void resize( index_type width, index_type height ) { data.resize( boost::extents[width][height] ); }
		};
	}
}

#endif //ndef IG_INDUSTRY_GRAPHICS_IMAGE
