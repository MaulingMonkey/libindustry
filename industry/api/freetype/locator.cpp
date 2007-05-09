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
				static const boost::regex description_format( "^(?:(\\d+)pt +)?(.+?)(?:\\s+(bold)|\\s+(italic))*$" );
				boost::smatch             description_match;
				if (!regex_match(description.begin(),description.end(),description_match,description_format)) throw std::runtime_error( "Invalid font description format" );
				
				unsigned    size = 12;
				if ( description_match[1].matched ) size = boost::lexical_cast< unsigned >( std::string(description_match[1].first,description_match[1].second) );

				std::string name( description_match[2].first , description_match[2].second );
				std::transform( name.begin(), name.end(), name.begin(), std::tolower );

				bool bold=false, italic=false;
				for ( unsigned flag = 3 ; description_match[flag].matched ; flag += 1 ) {
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
				static const boost::regex w32_reg_format( "^(.+?)(?:\\s+(bold)|\\s+(italic))*\\s+\\(truetype\\)$" );
				boost::smatch             w32_reg_match;

				using namespace industry::api::windows;
				registry::key fonts = registry::registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ];

				face_info best = { "", 0, false, false };
				int best_score = std::numeric_limits<int>::min();
				BOOST_FOREACH( registry::value w32_font , fonts.values() ) {
					std::string w32_font_desc_ = w32_font.name();
					std::transform( w32_font_desc_.begin(), w32_font_desc_.end(), w32_font_desc_.begin(), std::tolower );
					const std::string& w32_font_desc = w32_font_desc_;

					if (!regex_search( w32_font_desc.begin(), w32_font_desc.end(), w32_reg_match, w32_reg_format )) continue; //malformed registry value
					std::string w32_font_name( w32_reg_match[1].first , w32_reg_match[1].second );
					//std::transform( w32_font_name.begin(), w32_font_name.end(), w32_font_name.begin(), std::tolower );

					if ( w32_font_name != name ) continue;
					bool w32_bold=false, w32_italic=false;
					for ( unsigned flag = 2 ; w32_reg_match[flag].matched ; flag += 1 ) {
						std::string w32_flag( w32_reg_match[flag].first, w32_reg_match[flag].second );
						if      ( w32_flag == "bold"   ) w32_bold   = true;
						else if ( w32_flag == "italic" ) w32_italic = true;
						else assert(!"reached");
					}
					int score = ((w32_italic==italic)?1:0) + ((w32_bold==bold)?2:0);
					face_info info = { "%WINDIR%\\Fonts\\" + w32_font.to_string(), size, bold, italic };
					if ( score <= best_score ) continue;
					best=info, best_score=score;
				}
				if ( best.filename.empty() ) throw std::runtime_error( "Could not find font" );
				return best;
#else
				throw std::runtime_error( "Unimplemented" );
#endif
			}
		}
	}
}
