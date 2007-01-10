// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Dec 25, 2006 - industry.type.hpp => industry/type.hpp
// May 27, 2006 - Bugfix (missing return keyword :S) & stupid warning supression
// May 21, 2006 - Created

#ifndef IG_INDUSTRY_TYPE
#define IG_INDUSTRY_TYPE

#include <typeinfo>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4800 ) //forcing int => bool performance warning
#endif //def _MSC_VER

namespace industry {
	class type {
		const std::type_info * info;
	public:
		type( void ) : info( 0 ) {}
		type( const std::type_info & src ) : info( & src ) {}
		type( const type & other ) : info( other.info ) {}
		
		friend bool operator==( const type & lhs , const type & rhs ) {
			return *(lhs.info) == *(rhs.info);
		}
		friend bool operator< ( const type & lhs , const type & rhs ) {
			return lhs.info->before( *(rhs.info) );
		}
	};
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif //def _MSC_VER

#endif //ndef IG_INDUSTRY_TYPE
