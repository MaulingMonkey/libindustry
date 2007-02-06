// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_ALGORITHM_CALL
#define IG_INDUSTRY_ALGORITHM_CALL

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <functional>

namespace industry {
	namespace algorithm {
		template < typename Self > class endpoint_processor {}; //temporary cruft

		template < typename Functor >
		class call_processor : public endpoint_processor< call_processor< Functor > > {
			Functor f;
		public:
			call_processor( const Functor & f ): f(f) {}

			template < typename PreceedingProcessor >
			void act( const PreceedingProcessor & p ) {
				BOOST_STATIC_ASSERT(( boost::is_same< typename PreceedingProcessor::processor_type , active_processor_tag >::value ));
				for ( ; !p.end() ; p.advance() ) f( p.get() );
			}
		};

		template < typename R , typename A1 > call_processor< std::pointer_to_unary_function<A1,R> > call( R(*f)(A1) )         { return std::ptr_fun(f); }
		template < typename R , typename C  > call_processor< std::mem_fun_ref_t<R,C> >              call( R (C::*f)() )       { return std::mem_fun_ref(f); }
		template < typename R , typename C  > call_processor< std::const_mem_fun_ref_t<R,C> >        call( R (C::*f)() const ) { return std::mem_fun_ref(f); }
		template < typename Functor >         call_processor< Functor >                              call( Functor f ) {
			typedef typename Functor::result_type result_type;
			return f;
		}

		//FUTURE: call( R(*)(A1,A2) ) dispatching from std::pairs? (A1,...An) from boost::tuples?
		//FUTURE: call( R(C::*)(A1) ) dispatching from std::pairs? (A1,...An) from boost::tuples?
		//FUTURE: call( Functor ) with Functor::operator()(A1,A2) dispatching from std::pairs? (A1,...An) from boost::tuples?
	}
}

#endif //ndef IG_INDUSTRY_ALGORITHM_CALL
