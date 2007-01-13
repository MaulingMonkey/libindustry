// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan  2, 2007 - Created
//
// FIXME:  Adding " , crtp_self< user >" to user results in ambiguity errors

#include <industry/template/crtp.hpp>
#include <boost/test/unit_test.hpp>

namespace {
	namespace QUALIFY {
		using namespace ::industry::templates;
		
		template < typename Self > struct class1 {};
		template < typename Self , typename , typename > struct class2 : crtp_self< Self > {};
		template < typename Self , typename > struct class3 {};
			
		class user;
			
		#define INHERIT crtp_inherit< Self , crtp<QUALIFY::class1> , crtp2<QUALIFY::class2,int,char> , crtp1<QUALIFY::class3,bool> >
		template < typename Self >
		class base : public INHERIT {
			typedef INHERIT inherit;
		#undef INHERIT
			/* GOTCHYA:  If extensions are not disabled under VS2005, the crtp_inherit inherited classes, in their untemplated forms,
			 *           under the same name, will be brought into the local scope.  Thus, without the qualification QUALIFY:: in the
			 *           above examples, class2 will match the completed class2<Self,int,char> rather than the template class2, causing
			 *           compiler errors as class2<Self,int,char> is not the template-template parameter crtp2 is expecting.
			 *
			 *           Spent a few days debugging this ****ing extension.  Should be documented when CRTP is.
			 *
			 *           Do not remove this comment, under penalty of massive stabbing wounds in the groin from yours truely.
			 */
			using inherit::self;
			typedef typename inherit::super3  super3;
		public:
			virtual ~base() {}
			void test() {
				BOOST_CHECK(( typeid(self())                                 == typeid(user)   ));
				BOOST_CHECK(( typeid(QUALIFY::class2<user,int,char>::self()) == typeid(user)   ));
				BOOST_CHECK(( typeid(QUALIFY::class3<Self,bool>)             == typeid(super3) ));
			}
		};
			
		class user : public base<user> {};
		std::auto_ptr< user > example( new user );
	}
}

void test_template_crtp() {
	using namespace QUALIFY;
	example->test();
	BOOST_CHECK((  dynamic_cast< class1<user>          * >( example.get() ) ));
	BOOST_CHECK((  dynamic_cast< class2<user,int,char> * >( example.get() ) ));
	BOOST_CHECK((  dynamic_cast< class3<user,bool>     * >( example.get() ) ));
	BOOST_CHECK(( !dynamic_cast< ::industry::nil       * >( example.get() ) ));
}
