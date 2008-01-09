// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS
#define IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS

#include <industry/sfinae.hpp>
#include <industry/nil.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/static_assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits.hpp>

namespace industry {
	namespace utility {
		// Implementation roster:
		// T get_at_type           < T >   ( Tuple )
		// T get_at_type_or_default< T >   ( Tuple , default )
		// T get_if_type           < Pred >( Tuple )
		// T get_if_type_or_default< Pred >( Tuple , default )

		namespace detail {
			template < typename Tuple, typename T > struct tuple_has_type {
				enum { value
					=  boost::is_same< typename Tuple::head_type , T >::value
					|| tuple_has_type< typename Tuple::tail_type , T >::value
				};
			};
			template < typename Tuple, typename T > struct tuple_has_many_of_type {
				enum { value
					=  boost::is_same< typename Tuple::head_type , T >::value
					?  tuple_has_type< typename Tuple::tail_type , T >::value
					:  tuple_has_many_of_type< typename Tuple::tail_type , T >::value
				};
			};
			template < typename Tuple, typename T > struct tuple_has_one_of_type {
				enum { value
					=  boost::is_same< typename Tuple::head_type , T >::value
					? !tuple_has_type< typename Tuple::tail_type , T >::value
					:  tuple_has_one_of_type< typename Tuple::tail_type , T >::value
				};
			};
			template < typename T > struct tuple_has_type        < boost::tuples::null_type , T > { enum { value = false }; };
			template < typename T > struct tuple_has_many_of_type< boost::tuples::null_type , T > { enum { value = false }; };
			template < typename T > struct tuple_has_one_of_type < boost::tuples::null_type , T > { enum { value = false }; };

			template < typename Tuple, template < typename > class P > struct tuple_has_pred_type {
				enum { value
					=  P< typename Tuple::head_type >::value
					|| tuple_has_pred_type< typename Tuple::tail_type , P >::value
				};
			};
			template < typename Tuple, template < typename > class P > struct tuple_has_pred_many_of_type {
				enum { value
					=  P< typename Tuple::head_type >::value
					?  tuple_has_pred_type< typename Tuple::tail_type , P >::value
					:  tuple_has_pred_many_of_type< typename Tuple::tail_type , P >::value
				};
			};
			template < typename Tuple, template < typename > class P > struct tuple_has_pred_one_of_type {
				enum { value
					=  P< typename Tuple::head_type >::value
					? !tuple_has_pred_type< typename Tuple::tail_type , P >::value
					:  tuple_has_pred_one_of_type< typename Tuple::tail_type , P >::value
				};
			};
			template < template < typename > class P > struct tuple_has_pred_type        < boost::tuples::null_type , P > { enum { value = false }; };
			template < template < typename > class P > struct tuple_has_pred_many_of_type< boost::tuples::null_type , P > { enum { value = false }; };
			template < template < typename > class P > struct tuple_has_pred_one_of_type < boost::tuples::null_type , P > { enum { value = false }; };

			template < typename Tuple, typename T > struct tuple_has_head_type { enum { value = false }; };
			template < typename Tuple > struct tuple_has_head_type< Tuple, typename Tuple::head_type > { enum { value = true }; };

			template < typename Tuple, template < typename > class Pred > struct tuple_meets_head_type_predicate { enum { value = Pred< typename Tuple::head_type >::value }; };
			template < template < typename > class Pred > struct tuple_meets_head_type_predicate< boost::tuples::null_type , Pred > { enum { value = false }; };

			template < typename T , typename Tuple , bool match = detail::tuple_has_head_type< Tuple, T >::value > struct tuple_get_at;
			template < template < typename > class P , typename Tuple , bool match = detail::tuple_meets_head_type_predicate< Tuple, P >::value > struct tuple_get_if_type;
			template < template < typename > class P , typename D , typename Tuple , bool match = detail::tuple_meets_head_type_predicate< Tuple, P >::value > struct tuple_get_if_type_or_default;
			


			template < typename T , typename Tuple > struct tuple_get_at<T, Tuple, false> {
				static T from( const Tuple& tuple ) {
					return tuple_get_at< T , typename Tuple::tail_type >::from( tuple.get_tail() );
				}
				static T from( const Tuple& tuple, const T& default_ ) {
					return tuple_get_at< T , typename Tuple::tail_type >::from( tuple.get_tail(), default_ );
				}
			};
			template < template < typename > class P , typename Tuple > struct tuple_get_if_type<P, Tuple, false> {
				BOOST_STATIC_ASSERT(( detail::tuple_has_pred_one_of_type< Tuple , P >::value )); //TODO: Cut down on spam here!
				typedef typename tuple_get_if_type<P,typename Tuple::tail_type>::result_type result_type;
				static result_type from( const Tuple& tuple ) {
					return tuple_get_if_type< P , typename Tuple::tail_type >::from( tuple.get_tail() );
				}
			};
			template < template < typename > class P , typename D , typename Tuple > struct tuple_get_if_type_or_default<P, D, Tuple, false> {
				BOOST_STATIC_ASSERT(( !detail::tuple_has_pred_many_of_type< Tuple , P >::value )); //TODO: Cut down on spam here!
				typedef typename tuple_get_if_type_or_default<P,D,typename Tuple::tail_type>::result_type result_type;
				static result_type from( const Tuple& tuple, const D& default_ ) {
					return tuple_get_if_type_or_default< P , D , typename Tuple::tail_type >::from( tuple.get_tail(), default_ );
				}
			};



			template < typename T , typename Tuple > struct tuple_get_at<T,Tuple,true> {
				static T from( const Tuple& tuple )                        { return tuple.get_head(); }
				static T from( const Tuple& tuple, const T& /*default_*/ ) { return tuple.get_head(); }
			};
			template < template < typename > class P , typename Tuple > struct tuple_get_if_type<P,Tuple,true> {
				typedef typename Tuple::head_type result_type;
				static result_type from( const Tuple& tuple ) { return tuple.get_head(); }
			};
			template < template < typename > class P , typename D , typename Tuple > struct tuple_get_if_type_or_default<P,D,Tuple,true> {
				typedef typename Tuple::head_type result_type;
				static result_type from( const Tuple& tuple, const D& /*default*/ ) { return tuple.get_head(); }
			};



			template < typename T > struct tuple_get_at< T , boost::tuples::null_type , false > {
				static T from( const boost::tuples::null_type& /*tuple*/, const T& default_ ) { return default_; }
			};
			template < template < typename > class P , typename D > struct tuple_get_if_type_or_default< P , D , boost::tuples::null_type , false > {
				typedef D result_type;
				static result_type from( const boost::tuples::null_type& /*tuple*/, const D& default_ ) { return default_; }
			};
		}

		template < typename T , typename Tuple > T get_at_type( const Tuple& tuple ) {
			BOOST_STATIC_ASSERT(( detail::tuple_has_one_of_type< Tuple , T >::value ));
			return detail::tuple_get_at<T,Tuple>::from(tuple);
		}
		template < typename T , typename Tuple > T get_at_type_or_default( const Tuple& tuple, const T& default_ ) {
			BOOST_STATIC_ASSERT(( !detail::tuple_has_many_of_type< Tuple , T >::value ));
			return detail::tuple_get_at<T,Tuple>::from(tuple,default_);
		}

		template < template < typename > class P , typename Tuple >
		typename detail::tuple_get_if_type<P,Tuple>::result_type get_if_type( const Tuple& tuple ) {
			BOOST_STATIC_ASSERT(( detail::tuple_has_pred_one_of_type< Tuple , P >::value ));
			return detail::tuple_get_if_type<P,Tuple>::from(tuple);
		}
		template < template < typename > class P , typename D , typename Tuple >
		typename detail::tuple_get_if_type_or_default<P,D,Tuple>::result_type get_if_type_or_default( const Tuple& tuple, const D& default_ ) {
			BOOST_STATIC_ASSERT(( !detail::tuple_has_pred_many_of_type< Tuple , P >::value ));
			return detail::tuple_get_if_type_or_default<P,D,Tuple>::from(tuple,default_);
		}
	}
}

#endif //ndef IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS
