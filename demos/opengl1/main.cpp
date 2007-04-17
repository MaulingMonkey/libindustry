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
#include <industry/api/opengl/texcoord.hpp>
#include <industry/api/opengl/texture.hpp>
#include <industry/api/opengl/vertex.hpp>
#include <industry/api/opengl/vbo.hpp>
#include <industry/pod/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <cmath>

const double pi = 3.141592653589;

namespace opengl = ::industry::api::opengl;
namespace pod    = ::industry::pod;

opengl::texture<2> generate_test_texture() {
	const size_t size = std::min( opengl::max_texture_size() , 256u );
	const size_t tiles = 3;
	boost::multi_array< opengl::color3ub, 2 > data( boost::extents[size][size] );

	for ( unsigned x = 0 ; x < size ; ++x ) {
		for ( unsigned y = 0 ; y < size ; ++y ) {
			const double r = (std::sin( x*2*pi/size * tiles )+1)  /  2  *  255.0;
			const double g = (std::sin( y*2*pi/size * tiles )+1)  /  2  *  255.0;
			const double b = ( (x+y)*255.0 * tiles )   /  2   /  size;

			data[x][y] = opengl::color3ub( GLubyte(r), GLubyte(g), GLubyte(b) );
		}
	}
	return opengl::texture<2>(data);
}

opengl::display_list generate_test_list() {
	using namespace opengl;

	static texture<2> test_texture = generate_test_texture();
	
	pod::tuple< texcoord2f , vertex2f > data[] = {
		{0.0f, 0.0f, -100.0f, -100.0f},
		{0.0f, 1.0f, -100.0f, +100.0f},
		{1.0f, 1.0f, +100.0f, +100.0f},
		{1.0f, 0.0f, +100.0f, -100.0f},
	};

	return display_list( GL_QUADS, data, test_texture );
}

int main () {
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
	SDL_SetVideoMode( 800 , 600 , 32 , SDL_OPENGL );

	using namespace industry::api;

	opengl::display_list example = generate_test_list();

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
		glColor3f( 1.0f, 1.0f, 1.0f );
		glCallList( example );

		SDL_GL_SwapBuffers();
	}
}
