
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	engine->input_mgr->add_listener( this );

	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );

	engine->ui_mgr->set_mouse_visible( true );
}

void layer_editor::pop()
{
	engine->input_mgr->remove_listener( this );

	engine->ui_mgr->set_mouse_visible( false );
}

void layer_editor::becoming_top_layer()
{
	game->viewport_caption = "Edit Mode";
}

void layer_editor::draw()
{
	w_layer::draw();

	game->draw_entities();

	// ----------------------------------------------------------------------------

	MATRIX
		->push_identity()
		->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 3 ), 10.0f ) )
		->translate( w_vec3( ( hover_tile.x * TILE_SZ ), -( hover_tile.y * TILE_SZ ), 0.0f ) );

	RENDER->draw( selector_bracket );

	MATRIX->pop();

	// ----------------------------------------------------------------------------

	MATRIX
		->push_identity()
		->translate( w_vec3( -v_window_hw, -(TILE_SZ * 4.0f), 100.0f ) );
	RENDER->draw_string( engine->ui_mgr->ui_font, s_format( "Current Room: %d", game->current_room ) );
	MATRIX->pop();
}

void layer_editor::on_listener_event_received( e_event_id event, void* object )
{
	const w_input_event* evt = static_cast<w_input_event*>( object );

	switch( event )
	{
		case e_event_id::input_motion:
		{
			switch( evt->input_id )
			{
				case e_input_id::mouse:
				{
					// todo : these magic numbers should go into some sort of rect or consts somewhere
					if( engine->input_mgr->mouse_vwindow_pos.y > 32 && engine->input_mgr->mouse_vwindow_pos.y < 32 + (TILE_SZ * 9) )
					{
						tile_from_screen_pos( engine->input_mgr->mouse_vwindow_pos.x, engine->input_mgr->mouse_vwindow_pos.y );
					}
				}
				break;
			}
		}
		break;

		case e_event_id::input_pressed:
		{
			switch( evt->input_id )
			{
				case e_input_id::controller_button_dpad_left:
				case e_input_id::keyboard_left:
				{
					if( evt->alt_down )
					{
						game->current_room--;
						game->current_room = w_max( game->current_room, 0 );
					}
					else
					{
						hover_tile.x--;
						hover_tile.x = w_max( hover_tile.x, 0 );
					}
				}
				break;

				case e_input_id::controller_button_dpad_right:
				case e_input_id::keyboard_right:
				{
					if( evt->alt_down )
					{
						game->current_room++;
						game->current_room = w_min( game->current_room, static_cast<int>( game->rooms.size() - 1 ) );
					}
					else
					{
						hover_tile.x++;
						hover_tile.x = w_min( hover_tile.x, 18 );
					}
				}
				break;

				case e_input_id::controller_button_dpad_up:
				case e_input_id::keyboard_up:
				{
					hover_tile.y--;
					hover_tile.y = w_max( hover_tile.y, 0 );
				}
				break;

				case e_input_id::controller_button_dpad_down:
				case e_input_id::keyboard_down:
				{
					hover_tile.y++;
					hover_tile.y = w_min( hover_tile.y, 8 );
				}
				break;

				case e_input_id::keyboard_0:
				case e_input_id::keyboard_1:
				case e_input_id::keyboard_2:
				case e_input_id::keyboard_3:
				case e_input_id::keyboard_4:
				case e_input_id::keyboard_5:
				case e_input_id::keyboard_6:
				case e_input_id::keyboard_7:
				case e_input_id::keyboard_8:
				case e_input_id::keyboard_9:
				{
					game->current_room = static_cast<int>( evt->input_id ) - static_cast<int>( e_input_id::keyboard_0 );
				}
				break;
			}
		}
		break;
	}
}

// takes a position within the game viewport and converts it into
// a tile index within the current room

void layer_editor::tile_from_screen_pos( float xpos, float ypos )
{
	float tiles_x = TILE_SZ / 2;
	float tiles_y = TILE_SZ * 2.5;

	float tile_x = round(( xpos - tiles_x ) / (float)TILE_SZ);
	float tile_y = round(( ypos - tiles_y ) / (float)TILE_SZ);

	hover_tile = w_vec2( round( tile_x ), round( tile_y ) );
	hover_tile.x = w_clamp( hover_tile.x, 0, 18 );
	hover_tile.y = w_clamp( hover_tile.y, 0, 8 );
}
