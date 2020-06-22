
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_gameviewport::push()
{
	img_gradient = std::make_unique<a_image>();
	img_gradient->create_from_texture( "background_gradient" );
}

void layer_gameviewport::draw()
{
	w_layer::draw();

	engine->opengl->push_matrix();
	engine->opengl->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 2 ), -100 ) );
	{
		SCOPED_VAR( rs_color( w_color( 1.0, 1.0, 1.0, 0.25f ) ) );
		engine->render->draw( img_gradient.get(), w_sz( v_window_w, TILE_SZ * 2 ) );
	}
	engine->opengl->pop_matrix();

	{
		SCOPED_VAR( rs_color( W_COLOR_ORANGE ) );
		engine->render->draw_string( engine->get_asset<a_font>( "larger_font" ), w_vec3( 0, v_window_hh - ( TILE_SZ * 1 ), 200 ), "Endless Adventure Editor", e_align::centered );
	}

	engine->opengl->push_matrix();
	engine->opengl->translate( w_vec3( -v_window_hw, v_window_hh - (TILE_SZ * 3), 0 ) );
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
	
