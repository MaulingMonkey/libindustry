// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <SDL/SDL.h>
#undef main
#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/color.hpp>
#include <industry/api/opengl/display_list.hpp>
#include <industry/api/opengl/texture.hpp>
#include <industry/api/opengl/vbo.hpp>

int main () {
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
	SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );
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

		SDL_GL_SwapBuffers();
	}
}
