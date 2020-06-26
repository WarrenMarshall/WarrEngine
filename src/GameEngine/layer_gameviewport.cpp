
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

	// gradient

	MATRIX
		->push()
		->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 2 ), -100 ) );
	RENDER
		->begin()
		->push_color( w_color( 1.0, 1.0, 1.0, 0.15f ) )
		->draw( img_gradient.get(), w_sz( v_window_w, TILE_SZ * 2 ) )
		->end();
	MATRIX
		->pop();

	// title

	RENDER
		->begin()
		->push_color( W_COLOR_ORANGE )
		->draw_string( engine->get_asset<a_font>( "larger_font" ), w_vec3( 0, v_window_hh - ( TILE_SZ * 1 ), 200 ), "Endless Adventure Editor", e_align::centered )
		->end();

	// tiles

	w_matrix* mtx =
		MATRIX
		->push()
		->translate( w_vec3( -v_window_hw, v_window_hh - (TILE_SZ * 3), 0 ) );

	for( int y = 0 ; y < ROOM_H ; ++y )
	{
		for( int x = 0 ; x < ROOM_W ; ++x )
		{
			int idx = ( y * ROOM_W ) + x;

			RENDER->draw( game->get_tile( game->test_room.tiles[ idx ] )->img );
			mtx->translate( w_vec3( TILE_SZ, 0, 0 ) );
		}

		mtx->translate( w_vec3( -( TILE_SZ * ROOM_W ), 0, 0 ) );
		mtx->translate( w_vec3( 0, -TILE_SZ, 0 ) );
	}

	MATRIX->pop();
}
