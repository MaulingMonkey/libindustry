// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#define INDUSTRY_API_OPENGL_DISABLE_RECTANGULAR_TEXTURES
#define INDUSTRY_API_OPENGL_OVERRIDE_MAX_TEXTURE_SIZE             64
#define INDUSTRY_API_OPENGL_OVERRIDE_MAX_RECTANGULAR_TEXTURE_SIZE 64

#include <SDL/SDL.h>
#undef main
#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/canvas.hpp>
#include <iostream>




int main () {
	try {
		SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );
		ilInit();
		ilutInit();
		ilutRenderer( ILUT_OPENGL );

		using namespace industry::api;
		opengl::canvas example(  "..\\demos\\opengl1\\libindustry.png" );

		double tween = 0.0f;

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
			glOrtho( 0 , 800 , 0 , 600 , -1 , +1 );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			using namespace opengl;
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

			example.render();

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
