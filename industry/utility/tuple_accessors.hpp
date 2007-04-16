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

			template < typename T , typename Tuple , bool equal = boost::is_same< T , typename Tuple::head_type >::value > struct tuple_get_at {
				static T from( const Tuple& tuple ) {
					return tuple_get_at< T , typename Tuple::tail_type >::from( tuple.get_tail() );
				}
				static T from( const Tuple& tuple, const T& default_ ) {
					return tuple_get_at< T , typename Tuple::tail_type >::from( tuple.get_tail(), default_ );
				}
			};

			template < typename T , typename Tuple > struct tuple_get_at< T , Tuple , true > {
				static T from( const Tuple& tuple )                        { return tuple.get_head(); }
				static T from( const Tuple& tuple, const T& /*default_*/ ) { return tuple.get_head(); }
			};

			template < typename T > struct tuple_get_at< T , boost::tuples::null_type , false > {
				static T from( const boost::tuples::null_type& /*tuple*/, const T& default_ ) { return default_; }
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
	}
}

#endif //ndef IG_INDUSTRY_UTILITY_TUPLE_ACCESSORS
