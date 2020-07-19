
#include "master_pch.h"
#include "master_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::draw()
{
	static a_subtexture* subtex = engine->get_asset<a_subtexture>( "tile_player" );

	MATRIX->push_identity();
	//MATRIX->top()->translate( { 16.0f, 8.0f } );
	//RENDER->warren_draw( subtex, w_rect( 0, 0, 64, 64 ) );
	RENDER->begin();
	//RENDER->warren_draw( subtex, w_rect( 32, -32, -1, -1 ) );
	RENDER->draw_sprite( subtex, w_rect( 16, 8 ) );
	RENDER->draw_sprite( subtex, w_rect( 32, 32 ) );
	RENDER->draw_sprite( subtex, w_rect( v_window_w, v_window_h ) );
	RENDER->end();
	MATRIX->pop();
}
