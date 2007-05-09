// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/config.hpp>
#include <industry/api/freetype/face.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#ifdef INDUSTRY_OS_WINDOWS
#include <industry/api/windows/registry.hpp>
#endif

/****************

irb(main):023:0> r = /^((\d+)pt +)?(.*?)( +(bold|italic|underlined))*$/
=> /^((\d+)pt +)?(.*?)( +(bold|italic|underlined))*$/

irb(main):024:0> "12pt courier bold".scan r
=> [["12pt ", "12", "courier", " bold", "bold"]]

****************/

namespace industry {
	namespace api {
		namespace freetype {
			face::info face::find_info_for( const std::string& fontname ) {
#ifdef INDUSTRY_OS_WINDOWS
				//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts [ Courier New (TrueType) = COUR.TTF ]
				using namespace industry::api::windows;
				registry::key fonts = registry::registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ];
				static const boost::regex re( "^((:digit:+)pt:space:+)?(.+?)(:space:+(bold|italic))*$" );
				BOOST_FOREACH( registry::value font , fonts.values() ) {
					const std::string& str = font.name();
					boost::smatch match;
					if (!regex_search( str.begin(), str.end(), match, re )) continue;
					
					unsigned size = 12; // TODO: Externalize default
					if ( match[2].matched ) { //size specified
						size = boost::lexical_cast< unsigned >( std::string( match[2].first , match[2].second ) );
					}

					assert( match[3].matched );
					std::string name( match[3].first , match[3].second );
					if ( name != str ) continue;

					bool bold = false, italic = false;
					for ( unsigned flag = 5 ; match[flag].matched ; flag += 2 ) {
						std::string flag_text( match[flag].first , match[flag].second );
						if ( flag_text == "bold" ) {
							if (bold) continue; //malformed registry value
							else bold = true;
						} else if ( flag_text == "italic" ) {
							if (italic) continue; //malformed registry value
							else italic = true;
						} else {
							assert(!"reached"); //should never occur
							continue; // assume malformd registry value confused the regex
						}
					}

					face::info info = { font.to_string(), size, bold, italic };
					return info;
				}
				throw std::runtime_error( "Could not find font" );
#else
				throw std::runtime_error( "Unimplemented" );
#endif
			}
			face::face( const boost::shared_ptr< detail::library_data >& library, const std::string& name ) {
				face::info info = find_info_for(name);
				//impl.reset( new detail::face_data( library , font.to_string().c_str() , 0 /*????*/ ) );
			}
		}
	}
}
