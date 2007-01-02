// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// Jan  2, 2007 - Created
// $LastChangedBy$ - $LastChangedDate$

// FIXME:  Adding " , crtp_self< user >" to user results in ambiguity errors

#include <industry/template/crtp.hpp>
#include <boost/test/unit_test.hpp>

namespace {
	using namespace ::industry::templates;
	
	template < typename Self > struct class1 {};
	template < typename Self , typename , typename > struct class2 : crtp_self< Self > {};
	template < typename Self , typename > struct class3 {};
	
	class user;
	
	#define INHERIT crtp_inherit< Self , crtp<class1> , crtp2<class2,int,char> , crtp1<class3,bool> >
	template < typename Self >
	class base : public INHERIT { typedef INHERIT inherit;
	#undef INHERIT
		using inherit::self;
		typedef typename inherit::super3  super3;
	public:
		base() {
			BOOST_CHECK(( typeid(self())                        == typeid(user)   ));
			BOOST_CHECK(( typeid(class2<user,int,char>::self()) == typeid(user)   ));
			BOOST_CHECK(( typeid(class3<Self,bool>)             == typeid(super3) ));
		}
		virtual ~base() {}
	};
	
	class user : public base<user> {};
}

void test_template_crtp() {
	user example;
	
	BOOST_CHECK((  dynamic_cast< class1<user>          * >( (user*) &example ) ));
	BOOST_CHECK((  dynamic_cast< class2<user,int,char> * >( (user*) &example ) ));
	BOOST_CHECK((  dynamic_cast< class3<user,bool>     * >( (user*) &example ) ));
	BOOST_CHECK(( !dynamic_cast< ::industry::nil       * >( (user*) &example ) ));
}
