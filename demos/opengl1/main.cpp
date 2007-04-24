// Copyright (c) 2007 Michael B. Edwin Rickert
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt ) 
//
// $LastChangedBy$ - $LastChangedDate$

#include <SDL/SDL.h>
#undef main
#include <industry/api/devil/import.hpp>
#include <industry/api/devil/image.hpp>
#include <industry/api/opengl/import.hpp>
#include <industry/api/opengl/color.hpp>
#include <industry/api/opengl/display_list.hpp>
#include <industry/api/opengl/texcoord.hpp>
#include <industry/api/opengl/texture.hpp>
#include <industry/api/opengl/vertex.hpp>
#include <industry/api/opengl/vbo.hpp>
#include <industry/pod/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <cmath>

const double pi = 3.141592653589;

namespace devil  = ::industry::api::devil;
namespace opengl = ::industry::api::opengl;
namespace pod    = ::industry::pod;

typedef opengl::texture< 2, opengl::unnormalized > test_texture_t;

test_texture_t generate_test_texture_1() {
	const size_t size  = std::min( opengl::max_texture_size() , 512u );
	const size_t tiles = 5;

	try {
		boost::multi_array< opengl::color3ub, 2 > data( boost::extents[size][size] );

		for ( unsigned x = 0 ; x < size ; ++x ) {
			for ( unsigned y = 0 ; y < size ; ++y ) {
				const double r = (std::sin( x*2*pi/size * tiles )+1)  /  2  *  255.0;
				const double g = (std::sin( y*2*pi/size * tiles )+1)  /  2  *  255.0;
				const double b = ( (x+y)*255.0 * tiles )   /  2   /  size;

				data[x][y] = opengl::color3ub( GLubyte(r), GLubyte(g), GLubyte(b) );
			}
		}
		return test_texture_t(data);
	} catch( ... ) {
		std::cout << "Error generating test texture of size: " << size << "x" << size << std::endl;
		throw;
	}
}

test_texture_t generate_test_texture_2() {
	devil::image logo( "..\\demos\\opengl1\\libindustry.png" );
	return test_texture_t(logo);
}

opengl::display_list generate_test_list_1() {
	using namespace opengl;

	test_texture_t test_texture_1 = generate_test_texture_1();
	
	pod::tuple< texcoord2f , vertex2f > data[] = {
		{0.0f, 0.0f, -100.0f, -100.0f},
		{0.0f, 1.0f, -100.0f, +100.0f},
		{1.0f, 1.0f, +100.0f, +100.0f},
		{1.0f, 0.0f, +100.0f, -100.0f},
	};

	return display_list( GL_QUADS, data, test_texture_1 );
}

opengl::display_list generate_test_list_2() {
	using namespace opengl;

	test_texture_t test_texture_2 = generate_test_texture_2();
	
	pod::tuple< texcoord2f , vertex2f > data[] = {
		{0.0f, 0.0f, -200.0f, -150.0f},
		{0.0f, 1.0f, -200.0f, +150.0f},
		{1.0f, 1.0f, +200.0f, +150.0f},
		{1.0f, 0.0f, +200.0f, -150.0f},
	};

	return display_list( GL_QUADS, data, test_texture_2 );
}

int main () {
	try {
		SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );
		ilInit();
		ilutInit();
		ilutRenderer( ILUT_OPENGL );

		using namespace industry::api;
		std::set< std::string > extensions = opengl::available_extensions();
		std::cout << "Extensions:\n\t";
		std::copy( extensions.begin(), extensions.end(), std::ostream_iterator<std::string>(std::cout,"\n\t") );
		std::cout << std::endl;

		opengl::display_list example1 = generate_test_list_1();
		opengl::display_list example2 = generate_test_list_2();

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
			glOrtho( -200 , +200 , -150 , +150 , -1 , +1 );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			using namespace opengl;
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

			tween += 0.0005f;
			while ( tween > 2*pi ) tween -= 2*pi;
			glColor4f( 1.0f, 1.0f, 1.0f, GLfloat((std::cos(tween)+1)/2) );

			glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
			glCallList( example1 );
			glBlendFunc( GL_ONE_MINUS_SRC_ALPHA , GL_SRC_ALPHA );
			glCallList( example2 );

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
