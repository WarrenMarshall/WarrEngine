
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_worldviewport::push()
{
	img_gradient = std::make_unique<a_subtexture>( "background_gradient" );
}

void layer_worldviewport::draw()
{
	// background

	MATRIX
		->push_identity()
		->translate( w_vec3( 0, 0, -900 ) );
	RENDER
		->begin()
		->push_color( w_color( 0.2f, 0.2f, 0.2f ) )
		->draw_sprite( engine->white_solid, w_sz( v_window_w, v_window_h ) )
		->end();
	MATRIX->pop();

	w_layer::draw();

	// gradient

	MATRIX
		->push()
		->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 2 ), -100.0f ) );
	RENDER
		->begin()
		->push_color( w_color( 1.0f, 1.0f, 1.0f, 0.75f ) )
		->draw( img_gradient.get(), w_sz( v_window_w, TILE_SZ * 2 ) )
		->end();
	MATRIX
		->pop();

	// title

	MATRIX->push_identity()
		->translate( w_vec3( 0.0f, v_window_hh - (TILE_SZ * 1.25f), 200.0f ) );
	RENDER
		->begin()
		->push_color( W_COLOR_ORANGE )
		->push_align( e_align::centered )
		->draw_string( engine->get_asset<a_font>( "larger_font" ), game->viewport_caption )
		->end();
	MATRIX->pop();

	// tiles

	MATRIX
		->push()
		->translate( w_vec3( -v_window_hw, v_window_hh - (TILE_SZ * 3), 0.0f ) );

	for( int y = 0 ; y < ROOM_H ; ++y )
	{
		MATRIX->push();
		for( int x = 0 ; x < ROOM_W ; ++x )
		{
			int idx = ( y * ROOM_W ) + x;

			RENDER->draw( game->get_tile( game->rooms[game->current_room].tiles[ idx ] )->img );
			MATRIX->top()->translate( w_vec3( TILE_SZ, 0, 0 ) );
		}
		MATRIX->pop()
			->translate( w_vec3( 0, -TILE_SZ, 0 ) );
	}

	MATRIX->pop();
}
