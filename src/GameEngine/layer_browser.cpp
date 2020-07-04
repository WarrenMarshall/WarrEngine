
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_browser::push()
{
	engine->input_mgr->add_listener( this );

	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void layer_browser::pop()
{
	engine->input_mgr->remove_listener( this );
}

void layer_browser::becoming_top_layer()
{
	game->viewport_caption = "Edit Mode";
}

void layer_browser::draw()
{
	w_layer::draw();

	game->draw_entities();

	MATRIX
		->push_identity()
		->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 3 ), 0.0f ) )
		->translate( w_vec3( ( hover_tile.x * TILE_SZ ), -( hover_tile.y * TILE_SZ ), 0.0f ) );

	RENDER->begin()
		->push_color( W_COLOR_TEAL )
		->draw( selector_bracket )
		->end();

	MATRIX->pop();
}

void layer_browser::on_listener_event_received( e_event_id event, void* object )
{
	const w_input_event* evt = static_cast<w_input_event*>( object );

	switch( event )
	{
		case e_event_id::input_motion:
		{
			tile_from_screen_pos( evt->mouse.pos.x, evt->mouse.pos.y );
			//log_msg( "%1.2f, %1.2f", evt->xpos, evt->ypos );
		}
		break;

		//case e_event_id::input_pressed:
		//{
		//	switch( evt->input_id )
		//	{
		//		case e_input_id::keyboard_9:
		//		{
		//			game->current_room = 9;
		//		}
		//		break;
		//	}
		//}
		//break;
	}
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
