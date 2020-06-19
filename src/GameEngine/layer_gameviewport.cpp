
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_gameviewport::push()
{
	img_gradient = std::make_unique<a_image>( "background_gradient" );
}

void layer_gameviewport::draw()
{
	w_layer::draw();

	engine->opengl->push_matrix();
	engine->opengl->translate( w_vec3( -v_window_hw, -v_window_hh, -100 ) );
	engine->render->draw( img_gradient.get(), w_sz( v_window_w, v_window_h ), w_color( 1.0, 1.0, 1.0, 0.15f ) );
	engine->opengl->pop_matrix();

	engine->opengl->push_matrix();
	engine->opengl->translate( w_vec3( -v_window_hw, v_window_hh - (TILE_SZ * 2), 0 ) );
	{
		for( int y = 0 ; y < ROOM_H ; ++y )
		{
			engine->opengl->push_matrix();
			for( int x = 0 ; x < ROOM_W ; ++x )
			{
				int idx = ( y * ROOM_W ) + x;

				engine->render->draw( game->get_tile( game->test_room.tiles[ idx ] )->img );
				engine->opengl->translate( w_vec3( TILE_SZ, 0, 0 ) );
			}
			engine->opengl->pop_matrix();

			engine->opengl->translate( w_vec3( 0, -TILE_SZ, 0 ) );
		}
	}
	engine->opengl->pop_matrix();
}
	
