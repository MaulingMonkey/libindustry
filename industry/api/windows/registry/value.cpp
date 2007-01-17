// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt )
//
// $LastChangedBy$ - $LastChangedDate$
//
// Jan 10, 2007 - Added to SVN, cleaned up
// Oct 27, 2006 - Created

#include <industry/config.hpp>

#if defined( INDUSTRY_OS_WINDOWS )
#include <industry/api/windows/registry/value.hpp>
#include <algorithm>
#include <limits>
#include <cassert>

namespace industry {
	namespace api {
		namespace windows {
			namespace registry {
				namespace {
					template < typename T > struct raw_reader_policy {
						typedef T data_element_type;
						typedef T data_type;

						T data;

						size_t bytes() const { return sizeof(T); }
						void resize_bytes( unsigned ) { throw invalid_value_type_error(); }
						void aggressive_resize_bytes( unsigned ) { throw invalid_value_type_error(); }
						const T * begin() const { return &data;   }
						const T * end()   const { return &data+1; }
					};

					template < typename T > struct buffered_reader_policy {
						typedef T data_element_type;
						typedef std::vector< data_element_type > data_type;

						data_type data;
						buffered_reader_policy() : data( 16 ) {}

						size_t bytes() const { return data.size() * sizeof(T); }
						void resize_bytes( unsigned n ) {
							if ( n % sizeof(T) ) throw invalid_value_type_error();
							n /= sizeof(T);
							data.resize( n );
						}
						void aggressive_resize_bytes( unsigned n ) {
							if ( n % sizeof(T) ) throw invalid_value_type_error();
							n /= sizeof(T);
							if ( n > data.capacity() ) data.resize(n) , data.resize( data.capacity() );
							else data.resize( data.capacity() );
						}
						const T * begin() const { return data.empty() ? NULL : &(data[0]);   }
						const T * end()   const { return data.empty() ? NULL : &(data[0]) + data.size(); }
					};

					template < typename reader_policy > struct raw_policy : reader_policy {
						using typename reader_policy::data_type;
						const data_type & interpret( DWORD type ) const {
							if ( type == REG_LINK || type == REG_NONE ) throw invalid_value_type_error();
							return data;
						}
					};

					template < typename reader_policy > struct raw_endian_swapped_policy : reader_policy {
						using typename reader_policy::data_type;
						const data_type & interpret( DWORD type ) const {
							if ( type == REG_LINK || type == REG_NONE ) throw invalid_value_type_error();
							if ( type != REG_DWORD_BIG_ENDIAN ) return data;
							char * head = (char *)begin();
							char * tail = ((char *)end()) - 1;
							while ( head < tail ) std::swap( *head++ , *tail-- );
							return data;
						}
					};

					template < typename T > struct stringify_policy : buffered_reader_policy< TCHAR > {
						T interpret( DWORD type ) const {
							if (!( type == REG_EXPAND_SZ || type == REG_SZ )) throw invalid_value_type_error();
							return T( begin() , end()-1 );
						}
					};

					template < typename T > struct multi_string_policy;
					template < typename T > struct multi_string_policy /*< std::vector< T > >*/ : stringify_policy< T > {
						std::vector< T > interpret( DWORD type ) const {
							if ( type == REG_EXPAND_SZ || type == REG_SZ ) return std::vector< T >( 1 , stringify_policy< T >::interpret( type ) );
							if ( type != REG_MULTI_SZ ) throw invalid_value_type_error();
							std::vector< T > result;
							typename data_type::const_iterator s_begin , s_end;
							for ( s_begin = s_end = data.begin() ; ; data_element_type(0) ) {
								s_end = std::find( s_begin , data.end() , _T('\0') );
								if ( s_begin == s_end      ) return result;                 //an empty string marks the end of the list
								if ( s_end   == data.end() ) throw malformed_value_error(); //but reaching the end of the buffer before then marks a malformation
								result.push_back( T( s_begin , s_end ) );                   //otherwise, continue on as usual
							}
						}
					};


					template < typename T > struct registry_reader_f_base;
					template <> struct registry_reader_f_base< DWORD                       > : raw_endian_swapped_policy< raw_reader_policy< DWORD > > {};
					template <> struct registry_reader_f_base< QWORD                       > : raw_endian_swapped_policy< raw_reader_policy< QWORD > > {};
					template <> struct registry_reader_f_base< std::vector< char >         > : raw_policy< buffered_reader_policy< char > > {};
#if defined( UNICODE )
					template <> struct registry_reader_f_base< std::string                 > : stringify_policy< std::string > {};
					template <> struct registry_reader_f_base< std::vector< std::string  > > : multi_string_policy< std::string > {};
					template <> struct registry_reader_f_base< std::wstring                > : stringify_policy< std::wstring > {};
					template <> struct registry_reader_f_base< std::vector< std::wstring > > : multi_string_policy< std::wstring > {};
#else
					template <> struct registry_reader_f_base< std::string                 > : buffered_registry_reader_f_base< char > {};
					template <> struct registry_reader_f_base< std::vector< std::string  > > : buffered_registry_reader_f_base< char > {};
#endif

					template < typename T >
					struct registry_reader_f : registry_reader_f_base< T > {
						T operator()( HKEY key , const tstring & name ) {
							DWORD type;

							LONG error = ERROR_MORE_DATA;
							for ( unsigned tries = 0 ; error == ERROR_MORE_DATA ; ++tries ) {
								assert( bytes() <= std::numeric_limits< DWORD >::max() );
								DWORD size = DWORD( bytes() );
								error = ::RegQueryValueEx( /* hkey          */ key
														 , /* value name    */ name.c_str()
														 , /* reserved      */ NULL
														 , /* (out)    type */ &type
														 , /* (out)    data */ (LPBYTE) begin()
														 , /* (in/out) size */ &size
														 );
								if ( error == ERROR_SUCCESS && size != 0 && bytes() == 0 ) error = ERROR_MORE_DATA;
								if ( error == ERROR_SUCCESS   ) resize_bytes( size );
								if ( error == ERROR_MORE_DATA ) aggressive_resize_bytes( size );
							}
							throw_key_error( error );
							return interpret( type );
						}
					};
				}

				std::vector< char >         value::to_data     () const { return registry_reader_f< std::vector< char >         >()( *impl->key , impl->name ); }
				DWORD                       value::to_dword    () const { return registry_reader_f< DWORD                       >()( *impl->key , impl->name ); }
				QWORD                       value::to_qword    () const { return registry_reader_f< QWORD                       >()( *impl->key , impl->name ); }
				std::string                 value::to_string   () const { return registry_reader_f< std::string                 >()( *impl->key , impl->name ); }
				std::vector< std::string >  value::to_strings  () const { return registry_reader_f< std::vector< std::string >  >()( *impl->key , impl->name ); }
				tstring                     value::to_tstring  () const { return registry_reader_f< tstring                     >()( *impl->key , impl->name ); }
				std::vector< tstring >      value::to_tstrings () const { return registry_reader_f< std::vector< tstring >      >()( *impl->key , impl->name ); }
#if defined( UNICODE )
				std::wstring                value::to_wstring  () const { return registry_reader_f< std::wstring                >()( *impl->key , impl->name ); }
				std::vector< std::wstring > value::to_wstrings () const { return registry_reader_f< std::vector< std::wstring > >()( *impl->key , impl->name ); }
#endif
			}
		}
	}
}
#endif //defined( INDUSTRY_OS_WINDOWS )
