// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_GRAPHICS_COLOR
#define IG_INDUSTRY_GRAPHICS_COLOR

#include <boost/static_assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/array.hpp>

namespace industry {
	namespace graphics {
		struct grey {
		};
		typedef grey gray;
		struct rgb {
			template < typename SelfT , typename ComponentsT >
			class mixin_compound_channels {
				BOOST_STATIC_ASSERT(( ComponentsT::components >= 3 ));
				ComponentsT components;
			public:
				typedef typename ComponentsT::reference       reference;
				typedef typename ComponentsT::const_reference const_reference;
				const_reference red  () const { return components[0]; }
				const_reference green() const { return components[1]; }
				const_reference blue () const { return components[2]; }
				reference red  () { return components[0]; }
				reference green() { return components[1]; }
				reference blue () { return components[2]; }
			};
			template < typename SelfT , typename ComponentsT >
			struct mixin_component_channels {
				ComponentsT red, green, blue;
			};
		};
		struct rgba {
			template < typename SelfT , typename ComponentsT >
			class mixin_compound_channels {
				BOOST_STATIC_ASSERT(( ComponentsT::components >= 4 ));
				ComponentsT components;
			public:
				typedef typename ComponentsT::reference       reference;
				typedef typename ComponentsT::const_reference const_reference;
				const_reference red  () const { return components[0]; }
				const_reference green() const { return components[1]; }
				const_reference blue () const { return components[2]; }
				const_reference alpha() const { return components[3]; }
				reference red  () { return components[0]; }
				reference green() { return components[1]; }
				reference blue () { return components[2]; }
				reference alpha() { return components[3]; }
			};
			template < typename SelfT , typename ComponentsT >
			struct mixin_component_channels {
				ComponentsT red, green, blue, alpha;
			};
		};

#if 0
		class packed565 {
			unsigned short data;
		public:
			static const size_t components = 3;
			static const component_type value_min=0, clamp_min=0, clamp_max=1<<6, value_max=1<<6;
			typedef unsigned char component_type;

			class reference {
				packed565* self;
				unsigned index;
			public:
				reference( packed565* self, unsigned index ): self(self), index(index) {}
			};
		};
#endif //0

		template < size_t N >
		class packed8n {
			boost::array< unsigned char, N > data;
		public:
			static const size_t components = N;
			typedef unsigned char component_type;
			static const component_type value_min=0, clamp_min=0, clamp_max=255, value_max=255;
			typedef component_type&       reference;
			typedef const component_type& const_reference;

			      component_type& operator[]( size_t i )       { return data[i]; }
			const component_type& operator[]( size_t i ) const { return data[i]; }
		};
		BOOST_MPL_HAS_XXX_TRAIT_DEF(component_type);
		namespace detail {
			template < typename SelfT, typename ComponentsT, typename ChannelsT >
			struct mixin_compound_channels {
				typedef typename ChannelsT::template mixin_compound_channels< SelfT, ComponentsT > type;
			};
			template < typename SelfT, typename ComponentsT, typename ChannelsT >
			struct mixin_component_channels {
				typedef typename ChannelsT::template mixin_component_channels< SelfT, ComponentsT > type;
			};
		}

		typedef packed8n<1> packed8;
		typedef packed8n<2> packed88;
		typedef packed8n<3> packed888;
		typedef packed8n<4> packed8888;

		template < typename ComponentsT, typename ChannelsT >
		class color : public boost::mpl::eval_if< has_component_type<ComponentsT>
			, detail::mixin_compound_channels<  color<ComponentsT,ChannelsT>,  ComponentsT,  ChannelsT  >
			, detail::mixin_component_channels<  color<ComponentsT,ChannelsT>,  ComponentsT,  ChannelsT  >
			>::type
		{
		};
	}
}

#endif //ndef IG_INDUSTRY_GRAPHICS_COLOR
