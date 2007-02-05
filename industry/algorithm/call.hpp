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

		template < typename R > UNIMPLEMENTED call( R (*)() ) {}
		template < typename R , typename A1 > UNIMPLEMENTED call( R (*)(A1) ) {}
		template < typename R , typename A1 , typename A2 > UNIMPLEMENTED call( R (*)(A1,A2) ) {}

		template < typename R , typename C > UNIMPLEMENTED call( R (C::*)() ) {}
		template < typename R , typename C , typename A1 > UNIMPLEMENTED call( R (C::*)(A1) ) {}
		template < typename R , typename C , typename A1 , typename A2 > UNIMPLEMENTED call( R (C::*)(A1,A2) ) {}

		template < typename R , typename C > UNIMPLEMENTED call( R (C::*)() const ) {}
		template < typename R , typename C , typename A1 > UNIMPLEMENTED call( R (C::*)(A1) const ) {}
		template < typename R , typename C , typename A1 , typename A2 > UNIMPLEMENTED call( R (C::*)(A1,A2) const ) {}
	}
}
