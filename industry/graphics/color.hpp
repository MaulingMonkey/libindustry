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
#include <boost/preprocessor.hpp>
#include <boost/array.hpp>
#include <climits>

#define DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_TEMPLATES(r,d,i,channel) BOOST_PP_COMMA_IF(i) typename BOOST_PP_CAT(C,i)
#define DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_ARGS(r,d,i,channel) BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(C,i) channel
#define DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_ACCESSORS_EVAL(r,d,  channel) this->channel () = typename ComponentsT::component_type(channel);
#define DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_VALUES_EVAL(r,d,  channel) this->channel = ComponentT(channel);
#define DO_INDUSTRY_CREATE_COLOR_SPACE_MEMBERS(r,d,channel) ComponentT channel;
#define DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGNS(z,n,channels)
/*---------------------------------------------------------------------------------------------------------------*/
#define DO_INDUSTRY_CREATE_COLOR_SPACE_ACCESSORS(r,d,i,channel)                                                   \
	typename ComponentsT::template info< i >::reference       channel ()       { return components.get< i >(); }  \
	typename ComponentsT::template info< i >::const_reference channel () const { return components.get< i >(); }  \
/*---------------------------------------------------------------------------------------------------------------------*/
#define INDUSTRY_CREATE_COLOR_SPACE( name, channels )                                                                   \
	struct name {                                                                                                       \
		template < typename SelfT , typename ComponentsT >                                                              \
		class mixin_compound_channels {                                                                                 \
			BOOST_STATIC_ASSERT(( ComponentsT::components >= BOOST_PP_SEQ_SIZE(channels) ));                            \
			ComponentsT components;                                                                                     \
		public:                                                                                                         \
			BOOST_PP_SEQ_FOR_EACH_I( DO_INDUSTRY_CREATE_COLOR_SPACE_ACCESSORS, ~, channels )                            \
			template < BOOST_PP_SEQ_FOR_EACH_I( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_TEMPLATES, ~, channels ) >        \
			void assign( BOOST_PP_SEQ_FOR_EACH_I( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_ARGS, ~, channels ) ) {         \
				BOOST_PP_SEQ_FOR_EACH( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_ACCESSORS_EVAL, ~, channels )              \
			}                                                                                                           \
		};                                                                                                              \
		template < typename SelfT , typename ComponentT >                                                               \
		struct mixin_component_channels {                                                                               \
			BOOST_PP_SEQ_FOR_EACH( DO_INDUSTRY_CREATE_COLOR_SPACE_MEMBERS, ~, channels )                                \
			template < BOOST_PP_SEQ_FOR_EACH_I( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_TEMPLATES, ~, channels ) >        \
			void assign( BOOST_PP_SEQ_FOR_EACH_I( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_ARGS, ~, channels ) ) {         \
				BOOST_PP_SEQ_FOR_EACH( DO_INDUSTRY_CREATE_COLOR_SPACE_ASSIGN_VALUES_EVAL, ~, channels )                 \
			}                                                                                                           \
		};                                                                                                              \
	};                                                                                                                  \
/*---------------------------------------------------------------------------------------------------------------------*/

namespace industry {
	namespace graphics {
		INDUSTRY_CREATE_COLOR_SPACE( rgb , (red)(green)(blue)         );
		INDUSTRY_CREATE_COLOR_SPACE( rgba, (red)(green)(blue)(alpha)  );
		INDUSTRY_CREATE_COLOR_SPACE( rgbu, (red)(green)(blue)(unused) );

		template < typename DataT, unsigned N0, unsigned N1, unsigned N2, unsigned N3 = 0 >
		class packedNNNN {
			BOOST_STATIC_ASSERT(( !N3 || N2 ));
			BOOST_STATIC_ASSERT(( !N2 || N1 ));
			BOOST_STATIC_ASSERT(( N0 ));
			BOOST_STATIC_ASSERT(( N0 <= CHAR_BIT ));
			BOOST_STATIC_ASSERT(( N1 <= CHAR_BIT ));
			BOOST_STATIC_ASSERT(( N2 <= CHAR_BIT ));
			BOOST_STATIC_ASSERT(( N3 <= CHAR_BIT ));
			static const size_t bits = sizeof(DataT)*CHAR_BIT;
			BOOST_STATIC_ASSERT(( (N0+N1+N2+N3) == bits )); //no 555==16 pads -- explicit 1555 or 5551 only (use urgb/rgbu to determine unused bit side)
			template < size_t i > struct NN;
			template <> struct NN<0> { static const size_t value = N0; };
			template <> struct NN<1> { static const size_t value = N1; };
			template <> struct NN<2> { static const size_t value = N2; };
			template <> struct NN<3> { static const size_t value = N3; };
		public:
			typedef DataT data_type;
			data_type data;
			static const size_t components = (N0?1:0) + (N1?1:0) + (N2?1:0) + (N3?1:0);
			typedef unsigned char component_type;
		private:
			template < size_t i > struct info_ {
				static const unsigned lshift = NN<i+1>::value + info_<i+1>::lshift;
			};
			template <> struct info_< components-1 > {
				static const unsigned lshift = 0;
			};
		public:
			template < size_t i > class info {
				static const component_type mask = (NN<i>::value==CHAR_BIT) ? ~0u : ((1u<<NN<i>::value)-1);
			public:
				static const component_type value_min=0, clamp_min=0, clamp_max=mask, value_max=mask;
				class reference {
					packedNNNN* self;
				public:
					reference( packedNNNN* self ): self(self) {}
					operator component_type() const {
						component_type value = component_type((self->data >> info_<i>::lshift) & mask);
						return value;
					}
					reference& operator=( component_type value ) {
						unsigned _mask = mask;
						unsigned _lshift = info_<i>::lshift;
						assert( value <= value_max );
						self->data &= ~(DataT(mask) << info_<i>::lshift);
						self->data |= value << info_<i>::lshift;
						return *this;
					}
				};
				class const_reference {
					const packedNNNN* self;
				public:
					const_reference( const packedNNNN* self ): self(self) {}
					operator component_type() const {
						component_type value = self->data >> lshift_of<i>::value & mask;
					}
				};
			};
			template < size_t I > typename info<I>::reference       get()       { return info<I>::reference(this);       }
			template < size_t I > typename info<I>::const_reference get() const { return info<I>::const_reference(this); }
		};
		typedef packedNNNN< unsigned short, 1, 5, 5, 5 > packed1555;
		typedef packedNNNN< unsigned short, 5, 5, 5, 1 > packed5551;
		typedef packedNNNN< unsigned short, 5, 6, 5, 0 > packed565;

		template < size_t N >
		class packed8n {
		public:
			typedef boost::array< unsigned char, N > data_type;
			data_type data;
			static const size_t components = N;
			typedef unsigned char component_type;

			template < size_t I >       component_type& get()       { return data[I]; }
			template < size_t I > const component_type& get() const { return data[I]; }
			template < size_t I > struct info {
				static  const component_type value_min=0, clamp_min=0, clamp_max=255, value_max=255;
				typedef       component_type& reference;
				typedef const component_type& const_reference;
			};
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
		public:
			color() {}
			template < typename T0 > color( T0 a0 ) { assign(a0); }
			template < typename T0, typename T1 > color( T0 a0, T1 a1 ) { assign(a0,a1); }
			template < typename T0, typename T1, typename T2 > color( T0 a0, T1 a1, T2 a2 ) { assign(a0,a1,a2); }
			template < typename T0, typename T1, typename T2, typename T3 > color( T0 a0, T1 a1, T2 a2, T3 a3 ) { assign(a0,a1,a2,a3); }
		};
	}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif //def _MSC_VER

#endif //ndef IG_INDUSTRY_GRAPHICS_COLOR
