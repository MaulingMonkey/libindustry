// Copyright (c) 2008 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$

#ifndef IG_INDUSTRY_LANGUAGES_RUBY_ID
#define IG_INDUSTRY_LANGUAGES_RUBY_ID

#include <industry/languages/ruby/detail/wrap_retarded_ruby.hpp>
#include <boost/function.hpp>

#define INDUSTRY_RUBY_ID_LIST_ENTRY(r,prefix,id) const ::industry::languages::ruby::symbol prefix ## id ( #id );
#define INDUSTRY_RUBY_ID_LIST( prefix, sequence ) namespace { BOOST_PP_SEQ_FOR_EACH( INDUSTRY_RUBY_ID_LIST_ENTRY, prefix, sequence ) }

namespace industry { namespace languages { namespace ruby {
	class id {
		ID id_;
	public:
		id(): id_(0) {}
		id( const char* id ): id_(rb_intern(id)) {}
		template < size_t N > id( const char (&id)[N] ): id_(rb_intern2(id,N-1)) {} // don't include terminating '\0'
		id( const std::string& id ): id_(rb_intern2(id.c_str(),id.size())) {}
		id( const char* id, std::size_t N ): id_(rb_intern2(id,N)) {}

		ID get_id() const { return id_; }
	};
}}}

#endif //ndef IG_INDUSTRY_LANGUAGES_RUBY_ID
