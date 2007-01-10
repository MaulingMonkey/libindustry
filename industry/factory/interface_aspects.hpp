// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 26, 2006 - industry.factory.interface.aspects.hpp => industry/factory/interface_aspects.hpp
// May 31, 2006 - Created

#ifndef INDUSTRY_FACTORY_INTERFACE_ASPECT_IS_GENERATING
  #ifndef IG_INDUSTRY_FACTORY_INTERFACE_ASPECTS
  #define IG_INDUSTRY_FACTORY_INTERFACE_ASPECTS
  #include <boost/any.hpp>
  #include <boost/shared_ptr.hpp>
  #include <boost/preprocessor/repetition.hpp>
  #include <boost/variant.hpp>
  #include <industry/factory.hpp>

namespace industry {
	template < typename ReturnT , typename Method > struct return_coerced_interface_method_impl;
	template < typename ReturnT > struct return_coerced_interface_method_impl< ReturnT , void ( void ) > {
		typedef ReturnT (type)(void);
		typedef ReturnT return_type;
	};
	#define METHOD_IMPL(z,n,unused)                           \
	template < typename ReturnT , BOOST_PP_ENUM_PARAMS(n,typename A) >  \
	struct return_coerced_interface_method_impl               \
		< ReturnT , void ( BOOST_PP_ENUM_PARAMS(n,A) ) > {    \
		typedef ReturnT (type)( BOOST_PP_ENUM_PARAMS(n,A) );  \
		typedef ReturnT return_type;                          \
	};
	BOOST_PP_REPEAT_FROM_TO(1,INDUSTRY_FACTORY_METHOD_ARGUMENTS_LIMIT,METHOD_IMPL,~)
	#undef METHOD_IMPL

	
	template < typename ReturnT >
	struct return_coerced_interface {
		template < typename Method > struct method : return_coerced_interface_method_impl< ReturnT , Method > {};
	};

	namespace factory_detail {
		template < typename T , typename Method > struct boost_any_interface_aspect_impl;
		struct boost_any_interface : return_coerced_interface< boost::any > {
			template < typename T , typename Method > struct aspect : boost_any_interface_aspect_impl<T,Method> {
				using boost_any_interface_aspect_impl<T,Method>::create;
			};			
		};
		
		template < typename B , typename T , typename Method > struct boost_shared_ptr_interface_aspect_impl;
		template < typename B > struct boost_shared_ptr_interface : return_coerced_interface< boost::shared_ptr< B > > {
			template < typename T , typename Method > struct aspect : boost_shared_ptr_interface_aspect_impl<B,T,Method> {
				using boost_shared_ptr_interface_aspect_impl<B,T,Method>::create;
			};
		};
		
		template < typename B , typename T , typename Method > struct std_auto_ptr_interface_aspect_impl;
		template < typename B > struct std_auto_ptr_interface : return_coerced_interface< std::auto_ptr< B > > {
			template < typename T , typename Method > struct aspect : std_auto_ptr_interface_aspect_impl<B,T,Method> {
				using std_auto_ptr_interface_aspect_impl<B,T,Method>::create;
			};
		};
		
		#define VARIANT_TYPENAMES   BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , typename V )
		#define VARIANT_TYPES       BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , V )
		#define VARIANT_TYPE        boost::variant< VARIANT_TYPES >
		template < VARIANT_TYPENAMES , typename T , typename Method > struct boost_variant_interface_aspect_impl;
		template < VARIANT_TYPENAMES >
		struct boost_variant_interface : return_coerced_interface< VARIANT_TYPE > {
			template < typename T , typename Method > struct aspect
				: boost_variant_interface_aspect_impl<VARIANT_TYPES,T,Method> {
				using boost_variant_interface_aspect_impl<VARIANT_TYPES,T,Method>::create;
			};
		};
		#undef VARIANT_TYPENAMES
		#undef VARIANT_TYPES
		#undef VARIANT_TYPE
		
		template < typename B , typename T , typename Method > struct raw_ptr_interface_aspect_impl;
		template < typename B > struct raw_ptr_interface : return_coerced_interface< B * > {
			template < typename T , typename Method > struct aspect : raw_ptr_interface_aspect_impl<B,T,Method> {
				using raw_ptr_interface_aspect_impl<B,T,Method>::create;
			};
		};
	}
	template <> struct factory_interface< boost::any > {
		typedef factory_detail::boost_any_interface type;
	};
	
	template < typename B > struct factory_interface< boost::shared_ptr< B > > {
		typedef factory_detail::boost_shared_ptr_interface< B > type;
	};
	
	template < typename B > struct factory_interface< std::auto_ptr< B > > {
		typedef factory_detail::std_auto_ptr_interface< B > type;
	};
	
	template < BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , typename T ) >
	struct factory_interface< boost::variant< BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , T ) > > {
		typedef factory_detail::boost_variant_interface< BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , T ) > type;
	};
	
	template < typename B > struct factory_interface< B * > {
		typedef factory_detail::raw_ptr_interface< B > type;
	};
}

  #define INDUSTRY_FACTORY_INTERFACE_ASPECT_FILENAME <industry/factory/interface_aspects.hpp>
  #include <industry/factory/interface_aspect_gen.hpp>
  #undef INDUSTRY_FACTORY_INTERFACE_ASPECT_FILENAME
  #endif //ndef IG_INDUSTRY_FACTORY_INTERFACE_ASPECTS
#else //INDUSTRY_FACTORY_INTERFACE_ASPECT_IS_GENERATING

namespace industry {
	namespace factory_detail {
		template < typename T   TRAILING_TYPENAME_AN >
		struct boost_any_interface_aspect_impl< T , boost::any ( AN ) > {
			//typedef boost::any (function_type)( AN );
			static boost::any create( A_ARGN ) {
				return boost::any( T( ARGN ) );
			}
		};
		template < typename B , typename T  TRAILING_TYPENAME_AN >
		struct boost_shared_ptr_interface_aspect_impl< B , T , boost::shared_ptr< B > ( AN ) > {
			//typedef boost::shared_ptr< B > (function_type)( AN );
			static boost::shared_ptr< B > create( A_ARGN ) {
				return boost::shared_ptr< B >( new T( ARGN ) );
			}
		};
		template < typename B , typename T  TRAILING_TYPENAME_AN >
		struct std_auto_ptr_interface_aspect_impl< B , T , std::auto_ptr< B > ( AN ) > {
			static std::auto_ptr< B > create( A_ARGN ) {
				return std::auto_ptr< B >( new T( ARGN ) );
			}
		};
		
		#define VARIANT_TYPENAMES  BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , typename V )
		#define VARIANT_TYPES      BOOST_PP_ENUM_PARAMS( BOOST_VARIANT_LIMIT_TYPES , V )
		#define VARIANT_TYPE      boost::variant< VARIANT_TYPES >
		template < VARIANT_TYPENAMES , typename T  TRAILING_TYPENAME_AN >
		struct boost_variant_interface_aspect_impl< VARIANT_TYPES , T , VARIANT_TYPE ( AN ) > {
			static VARIANT_TYPE create( A_ARGN ) {
				return VARIANT_TYPE( T( ARGN ) );
			}
		};
		#undef VARIANT_TYPENAMES
		#undef VARIANT_TYPES
		#undef VARIANT_TYPE
		
		template < typename B , typename T  TRAILING_TYPENAME_AN >
		struct raw_ptr_interface_aspect_impl< B , T , B * ( AN ) > {
			static B * create( A_ARGN ) {
				return new T( ARGN );
			}
		};
	}
}

#endif //INDUSTRY_FACTORY_INTERFACE_ASPECT_IS_GENERATING
