// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <SDL/SDL.h>
#undef main
#include <industry/api/freetype/library.hpp>
#include <industry/api/opengl/canvas.hpp>
#include <industry/api/opengl/import.hpp>
#include <iostream>
#include <cmath>

namespace freetype = ::industry::api::freetype;
namespace opengl   = ::industry::api::opengl;

int main () {
	try {
		SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );

		freetype::library fonts;
		opengl::canvas canvas( 800, 600 );

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

			glClearColor( 0.0, 0.0, 0.0, 0.0 );
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
