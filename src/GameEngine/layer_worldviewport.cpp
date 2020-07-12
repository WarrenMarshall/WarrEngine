
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_worldviewport::push()
{
}

void layer_worldviewport::draw()
{
	w_layer::draw();

	// solid background

	MATRIX
		->push_identity()
		->translate( w_vec2( 0, 0 ) );
	RENDER
		->begin()
		->push_depth( -900 )
		->push_color( w_color( 0.2f, 0.2f, 0.2f ) )
		->draw_sprite( engine->white_solid, w_sz( v_window_w, v_window_h ) )
		->end();
	MATRIX->pop();

	// tiles

	MATRIX
		->push()
		->translate( w_vec2( -v_window_hw, v_window_hh - (TILE_SZ * 3) ) );

	for( int y = 0 ; y < ROOM_H ; ++y )
	{
		MATRIX->push();
		for( int x = 0 ; x < ROOM_W ; ++x )
		{
			int idx = ( y * ROOM_W ) + x;

			RENDER->draw( game->get_tile( game->rooms[game->current_room].tiles[ idx ] )->img );
			MATRIX->top()->translate( w_vec2( TILE_SZ, 0 ) );
		}
		MATRIX->pop()
			->translate( w_vec2( 0, -TILE_SZ ) );
	}

	MATRIX->pop();
}
