
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_browser::push()
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void layer_browser::pop()
{
}

void layer_browser::becoming_top_layer()
{
	game->viewport_caption = "Select A Tile";
}

void layer_browser::draw()
{
	w_layer::draw();


}

// takes a position within the game viewport and converts it into
// a tile index within the current room

int layer_browser::tile_from_screen_pos( float xpos, float ypos )
{
	float tiles_x = TILE_SZ / 2;
	float tiles_y = TILE_SZ * 2.5;

	float tile_x = round( ( xpos - tiles_x ) / (float) TILE_SZ );
	float tile_y = round( ( ypos - tiles_y ) / (float) TILE_SZ );

	hover_tile = w_vec2( round( tile_x ), round( tile_y ) );

	return 0;
}

void layer_browser::handle_input_event( const w_input_event* evt )
{
	log_msg( "Hi!" );
}