// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <industry/config.hpp>
#include <industry/api/freetype/locator.hpp>
#include <industry/api/windows/registry.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <algorithm>

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
			face_info locator::find_face_info( const std::string& description ) {
				static const boost::regex description_format( "^((\\d+)pt +)?(.+?)( +(bold|italic))*$" );
				boost::smatch             description_match;
				if (!regex_match(description.begin(),description.end(),description_match,description_format)) throw std::runtime_error( "Invalid font description format" );
				
				unsigned    size = 12;
				if ( description_match[2].matched ) size = boost::lexical_cast< unsigned >( std::string(description_match[2].first,description_match[2].second) );

				std::string name( description_match[3].first , description_match[3].second );
				std::transform( name.begin(), name.end(), name.begin(), std::tolower );

				bool bold=false, italic=false;
				for ( unsigned flag = 5 ; description_match[flag].matched ; flag += 2 ) {
					std::string flag_text( description_match[flag].first , description_match[flag].second );
					if ( flag_text == "bold" ) {
						if (bold) throw std::runtime_error( "Invalid font description -- only one \"bold\" allowed" );
						else bold = true;
					} else if ( flag_text == "italic" ) {
						if (italic) std::runtime_error( "Invalid font description -- only one \"italic\" allowed" );
						else italic = true;
					} else {
						assert(!"reached"); //should never occur
						throw std::runtime_error( "What the **** did you do" );
					}
				}


#ifdef INDUSTRY_OS_WINDOWS
				static const boost::regex win32_reg_format( "^(.+?) +\\(TrueType\\)$" );
				boost::smatch             win32_reg_match;

				using namespace industry::api::windows;
				registry::key fonts = registry::registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ];

				BOOST_FOREACH( registry::value w32_font , fonts.values() ) {
					const std::string& w32_font_desc = w32_font.name();
					if (!regex_search( w32_font_desc.begin(), w32_font_desc.end(), win32_reg_match, win32_reg_format )) continue; //malformed registry value
					std::string w32_font_name( win32_reg_match[1].first , win32_reg_match[1].second );
					std::transform( w32_font_name.begin(), w32_font_name.end(), w32_font_name.begin(), std::tolower );
					if ( w32_font_name != name ) continue;
					//else matched

					face_info info = { w32_font.to_string(), size, bold, italic };
					return info;
				}
				throw std::runtime_error( "Could not find font" );
#else
				throw std::runtime_error( "Unimplemented" );
#endif
			}
		}
	}
}
