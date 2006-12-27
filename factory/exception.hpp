// Copyright (c) 2006 Michael B. Edwin Rickert 
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Dec 26, 2006 - industry.factory.exception.hpp => industry/factory/exception.hpp
// May 31, 2006 - invalid_factory_type => bad_factory_type, implemented thrower
// May 27, 2006 - Created

#ifndef IG_INDUSTRY_FACTORY_EXCEPTION
#define IG_INDUSTRY_FACTORY_EXCEPTION

#include <exception>

namespace industry {
	struct bad_factory_type : std::exception {
		//thown by:   factory::create( id , ... )
		//thown when: there was no call to factory::type< T >() with typeid(T) == id
		bad_factory_type() throw() {}
		~bad_factory_type() throw() {}
		const char * what( void ) const throw() {
			return "Invalid factory type";
		}
	};
}

#endif //ndef IG_INDUSTRY_FACTORY_EXCEPTION
