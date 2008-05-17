// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_EVAL
#define IG_INDUSTRY_LANGUAGES_RUBY_EVAL

#include <industry/languages/ruby/array_iterator.hpp>
#include <industry/languages/ruby/value.hpp>
#include <boost/range.hpp>
#include <string>

namespace industry { namespace languages { namespace ruby {
	lazy_value eval( const char *       str );
	lazy_value eval( const std::string& str );

	lazy_value safe_eval( const char *       str );
	lazy_value safe_eval( const std::string& str );

	namespace detail {
		template < typename T > struct typed_eval_impl {
			static T call( const char*        str ) { return eval(str).to<T>(); }
			static T call( const std::string& str ) { return eval(str).to<T>(); }
			typedef T result_type;
		};
		template < typename T > struct typed_eval_impl<T[]> {
			static boost::iterator_range< array_iterator<T> > call( const char* str ) {
				value array = eval(str);
				return boost::iterator_range< array_iterator<T> >( array_iterator<T>::begin(array), array_iterator<T>::end(array) );
			}
			static boost::iterator_range< array_iterator<T> > call( const std::string& str ) {
				value array = eval(str);
				return boost::iterator_range< array_iterator<T> >( array_iterator<T>::begin(array), array_iterator<T>::end(array) );
			}
			typedef boost::iterator_range< array_iterator<T> > result_type;
		};
		template < typename T > struct typed_safe_eval_impl {
			static T call( const char*        str ) { return safe_eval(str).to<T>(); }
			static T call( const std::string& str ) { return safe_eval(str).to<T>(); }
			typedef T result_type;
		};
		template < typename T > struct typed_safe_eval_impl<T[]> {
#if 0 // safe_array_iterator not yet implemented
			static boost::iterator_range< safe_array_iterator<T> > call( const char* str ) {
				value array = eval(str);
				return boost::iterator_range< safe_array_iterator<T> >( safe_array_iterator<T>::begin(array), safe_array_iterator<T>::end(array) );
			}
			static boost::iterator_range< safe_array_iterator<T> > call( const std::string& str ) {
				value array = eval(str);
				return boost::iterator_range< safe_array_iterator<T> >( safe_array_iterator<T>::begin(array), safe_array_iterator<T>::end(array) );
			}
			typedef boost::iterator_range< safe_array_iterator<T> > result_type;
#endif
		};
	}

	template < typename T > typename detail::typed_eval_impl<T>::result_type eval( const char*        str ) { return detail::typed_eval_impl<T>::call(str); }
	template < typename T > typename detail::typed_eval_impl<T>::result_type eval( const std::string& str ) { return detail::typed_eval_impl<T>::call(str); }

	template < typename T > typename detail::typed_safe_eval_impl<T>::result_type safe_eval( const char*        str ) { return detail::typed_safe_eval_impl<T>::call(str); }
	template < typename T > typename detail::typed_safe_eval_impl<T>::result_type safe_eval( const std::string& str ) { return detail::typed_safe_eval_impl<T>::call(str); }
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_EVAL
