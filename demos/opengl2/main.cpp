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
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <algorithm>
#include <iostream>
#include <string>




int main () {
	try {
		SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );
		ilInit();
		ilutInit();
		ilutRenderer( ILUT_OPENGL );

		using namespace industry::api;
		using namespace industry::api::opengl;
		opengl::canvas example1(  "..\\data\\libindustry.png"  );
		opengl::canvas example2(  "..\\data\\libindustry.png"  );

		int x=600, y=450;

		boost::multi_array< color3ub, 2 > pixel( boost::extents[1][1] );
		pixel[0][0] = color3ub(0,0,0);

		boost::multi_array< color3ub, 2 > fill( boost::extents[800][100] );

		boost::timer timer;
		unsigned frames = 0;

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

			for ( unsigned i = 0 ; i < 5 ; ++i ) {
				switch ( rand()%4 ) {
				case 0: x += 1; break;
				case 1: x -= 1; break;
				case 2: y += 1; break;
				case 3: y -= 1; break;
				}

				if ( x < 0 || x >= 800 ) x = rand()%800;
				if ( y < 0 || y >= 600 ) y = rand()%600;
				example1.blit(x,y,pixel);
			}

			std::fill( fill.data() , fill.data() + fill.shape()[0] * fill.shape()[1] , color3ub( rand()%256 , rand()%256 , rand()%256 ) );
			example1.blit(0,0,fill);

			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			example2.render();
			example1.render();

			SDL_GL_SwapBuffers();
			++frames;
			if ( timer.elapsed() > 1 ) {
				std::string title = "FPS: " + boost::lexical_cast<std::string>( double(frames)/timer.elapsed() );
				frames = 0;
				timer.restart();
				SDL_WM_SetCaption( title.c_str(), title.c_str() );
			}
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
