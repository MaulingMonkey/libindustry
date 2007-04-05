// Copyright (c) 2006-2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <SDL/SDL.h>
#include <SDL/SDL_Opengl.h>
#undef main

int main () {
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
	SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );
	while( true ) {
		SDL_Event e;
		while ( SDL_PollEvent( &e ) ) {
			switch( e.type ) {
				case SDL_QUIT:
					SDL_Quit();
					return 0;
			}
		}
		SDL_GL_SwapBuffers();
	}
}
