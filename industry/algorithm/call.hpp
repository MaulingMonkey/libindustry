// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

namespace industry {
	namespace algorithm {
		typedef void UNIMPLEMENTED; //temporary cruft
		template < typename Self > class endpoint_processor {}; //temporary cruft

		template < typename Functor >
		class call_processor : public endpoint_processor< call_processor< Functor > > {
			Functor f;
		public:
			call_processor( const Functor & f ): f(f) {}

			template < typename PreceedingProcessor >
			void act( const PreceedingProcessor & p ) {
				for ( ; !p.end() ; p.advance() ) f( p.get() );
			}
		};

		template < typename R , typename A1 > UNIMPLEMENTED call( R (*)(A1)        ) {}
		template < typename R , typename C  > UNIMPLEMENTED call( R (C::*)()       ) {}
		template < typename R , typename C  > UNIMPLEMENTED call( R (C::*)() const ) {}
		template < typename Functor >         UNIMPLEMENTED call( Functor          ) {
			typedef typename Functor::result_type result_type;
		}

		//FUTURE: call( R(*)(A1,A2) ) dispatching from std::pairs? (A1,...An) from boost::tuples?
		//FUTURE: call( R(C::*)(A1) ) dispatching from std::pairs? (A1,...An) from boost::tuples?
		//FUTURE: call( Functor ) with Functor::operator()(A1,A2) dispatching from std::pairs? (A1,...An) from boost::tuples?
	}
}
