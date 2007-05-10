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
#include <string>
#include <cstdlib>

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
			struct font_description {
				std::string name;
				unsigned    size;
				bool bold, italic;
			};

			namespace {
				unsigned match_score( const font_description& lhs, const font_description& rhs ) {
					if ( lhs.name != rhs.name ) return 0;
					return 1 + ((lhs.italic==rhs.italic)?2:0) + ((lhs.bold==rhs.bold)?4:0);
				}

				// NOTE:  Error indicated with exceptions
				font_description parse_description( std::string description ) {
					std::transform( description.begin(), description.end(), description.begin(), std::tolower );
					font_description desc = {"",0,false,false};
					static const boost::regex description_format( "^(?:(\\d+)pt +)?(.+?)(?:\\s+(bold)|\\s+(italic))*$" );
					boost::match_results< std::string::iterator > description_match;
					if (!regex_match(description.begin(),description.end(),description_match,description_format)) throw std::runtime_error( "Invalid font description format" );
					
					desc.size = 12;
					if ( description_match[1].matched ) desc.size = boost::lexical_cast< unsigned >( std::string(description_match[1].first,description_match[1].second) );

					desc.name = std::string( description_match[2].first , description_match[2].second );
					std::transform( desc.name.begin(), desc.name.end(), desc.name.begin(), std::tolower );

					for ( unsigned flag = 3 ; description_match[flag].matched ; flag += 1 ) {
						std::string flag_text( description_match[flag].first , description_match[flag].second );
						if ( flag_text == "bold" ) {
							if (desc.bold) throw std::runtime_error( "Invalid font description -- only one \"bold\" allowed" );
							else desc.bold = true;
						} else if ( flag_text == "italic" ) {
							if (desc.italic) std::runtime_error( "Invalid font description -- only one \"italic\" allowed" );
							else desc.italic = true;
						} else {
							assert(!"reached"); //should never occur
							throw std::runtime_error( "What the **** did you do" );
						}
					}
					return desc;
				}

				// NOTE:  Error indicated with "" name
				font_description parse_w32_reg_description( std::string description ) {
					std::transform( description.begin(), description.end(), description.begin(), std::tolower );
					font_description          desc = {"",0,false,false};
					static const boost::regex description_format( "^(.+?)(?:\\s+(bold)|\\s+(italic))*\\s+\\(truetype\\)$" );
					boost::match_results< std::string::iterator > description_match;

					if (!regex_search( description.begin(), description.end(), description_match, description_format )) return desc;
					desc.name = std::string( description_match[1].first , description_match[1].second );
					std::transform( desc.name.begin(), desc.name.end(), desc.name.begin(), std::tolower );

					for ( unsigned flag = 2 ; description_match[flag].matched ; flag += 1 ) {
						std::string flag_text( description_match[flag].first, description_match[flag].second );
						if      ( flag_text == "bold"   ) desc.bold   = true;
						else if ( flag_text == "italic" ) desc.italic = true;
						else                              assert(!"reached");
					}
					return desc;
				}
			}

#ifdef INDUSTRY_OS_WINDOWS
			face_info locator::find_face_info( const std::string& target_description ) {
				font_description target = parse_description( target_description );

				face_info best_info  = { "", 0, target.size };
				unsigned  best_file_score=0, best_reg_score=0;

				using namespace industry::api::windows;
				registry::key fonts = registry::registry[ "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts" ];
				BOOST_FOREACH( registry::value font , fonts.values() ) {
					font_description reg_entry = parse_w32_reg_description( font.name() );
					if ( reg_entry.name.empty()        ) continue; // skip malformed registry entries
					std::string filename  = std::string(std::getenv("WINDIR")) + "\\Fonts\\" + font.to_string();
					if ( reg_entry.name != target.name ) continue; // skip misnamed  registry entries

					unsigned reg_score = match_score(target,reg_entry);
					if ( reg_score < best_reg_score ) continue;    // skip worse-match registry entries

					FT_Face face;
					if (FT_New_Face( library(), filename.c_str(), -1, &face )) continue; //unusable font file
					unsigned faces = face->num_faces;
					FT_Done_Face(face);
					for ( unsigned index = 0 ; index < faces ; ++index ) {
						FT_Error err = FT_New_Face( library(), filename.c_str(), index, &face );
						if ( err ) continue; //ignore errors in font files
						std::string style = face->style_name;
						std::transform( style.begin(), style.end(), style.begin(), std::tolower );
						font_description desc = { reg_entry.name, 0, false, false };
						if ( style.find( "bold"   ) != std::string::npos ) desc.bold   = true;
						if ( style.find( "italic" ) != std::string::npos ) desc.italic = true;
						FT_Done_Face( face );

						unsigned file_score = match_score(target,desc);
						if ( reg_score == best_reg_score && file_score <= best_file_score ) continue;
						// else reg_score > best_reg_score || file_score > best_file_score

						face_info info  = { filename, index , target.size };
						best_reg_score  = reg_score;
						best_file_score = file_score;
						best_info       = info;
					}
				}
				if ( best_info.filename.empty() ) throw std::runtime_error( "Could not find font" );
				return best_info;
			}
#endif
		}
	}
}
