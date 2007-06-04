// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <SDL/SDL.h>
#undef main
#include <industry/api/freetype.hpp>
#include <industry/api/opengl/canvas.hpp>
#include <industry/api/opengl/import.hpp>
#include <iostream>
#include <cmath>

namespace freetype = ::industry::api::freetype;
namespace opengl   = ::industry::api::opengl;
namespace fx       = ::industry::graphics;
namespace math     = ::industry::math;

int main () {
	try {
		SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );

		freetype::library fonts;
		freetype::face    courier( fonts, "12pt Courier New" );
		opengl::canvas    canvas( 800, 600 );
		std::vector< opengl::color1ub > greyness( 800*600, opengl::color1ub(0x10) );
		canvas.blit( 0, 0, 800, 600, &greyness[0] );

		fx::image< fx::greyscale<unsigned char> > data( 100, 100 );
		courier.char_blit( 'H', math::vector<int,2>(50,50), data );
		canvas.blit( 400, 300, 100, 100, data.data() );

		while( true ) {
			SDL_Event e;
			while ( SDL_PollEvent( &e ) ) {
				switch( e.type ) {
					case SDL_KEYDOWN:
						switch ( e.key.keysym.sym ) {
							case SDLK_ESCAPE:
								SDL_Quit();
								return 0;
							default:
								break;
						}
					case SDL_QUIT:
						SDL_Quit();
						return 0;
					default:
						break;
				}
			}

			glClearColor( 0.1f, 0.1f, 0.1f, 0.0 );
			glClear( GL_COLOR_BUFFER_BIT );

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			glOrtho( 0, 800 , 0 , 600 , -1 , +1 );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			canvas.render();

			SDL_GL_SwapBuffers();
		}
	} catch( const std::exception & e ) {
		std::cout << "Exception: " << e.what() << std::endl;
#ifdef _DEBUG
		throw;
#else
		return -1;
#endif
	}
}
