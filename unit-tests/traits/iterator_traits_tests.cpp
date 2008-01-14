// Copyright (c) 2006-2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Dec 28, 2006 - Created

#include <industry/traits/iterator_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <iterator>
#include <vector>

BOOST_AUTO_TEST_CASE( test_iterator_traits ) {
	typedef int * pointer;
	typedef const int * c_pointer;
	typedef std::istream_iterator< int > istream_iterator;
	typedef std::ostream_iterator< int > ostream_iterator;
	typedef std::vector< int >::iterator vector_iterator;
		
	typedef industry::iterator_traits< pointer >           pointer_traits;
	typedef industry::iterator_traits< c_pointer >         c_pointer_traits;
	typedef industry::iterator_traits< ostream_iterator >  ostream_iterator_traits;
	typedef industry::iterator_traits< istream_iterator >  istream_iterator_traits;
	typedef industry::iterator_traits< vector_iterator >   vector_iterator_traits;
	
	BOOST_STATIC_ASSERT(( boost::is_same<          pointer_traits::result_type ,       int& >::value ));
	BOOST_STATIC_ASSERT(( boost::is_same<        c_pointer_traits::result_type , const int& >::value ));
	BOOST_STATIC_ASSERT(( boost::is_same< istream_iterator_traits::result_type , const int& >::value ));
	BOOST_STATIC_ASSERT(( boost::is_same<  vector_iterator_traits::result_type ,       int& >::value ));
}
