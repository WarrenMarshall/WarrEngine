
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	engine->input_mgr->add_listener( this );

	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );

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

	if( draw_selector_bracket )
	{
		MATRIX
			->push_identity()
			->translate( w_vec3( -v_window_hw, v_window_hh - ( TILE_SZ * 3 ), 0 ) )
			->translate( w_vec3( ( hover_tile.x * TILE_SZ ), -( hover_tile.y * TILE_SZ ), 0 ) );

		RENDER->draw( selector_bracket );

		MATRIX->pop();
	}

	// ----------------------------------------------------------------------------

	/*
	float step_per_fts = tween_pingpong->step_per_sec / w_time::FTS_desired_frames_per_second;
	step_per_fts *= tween_pingpong->_dir;

	OPENGL
		->push( true )
		->add_transform( pingpong_xform )
		->translate( w_vec3( step_per_fts * RENDER->frame_interpolate_pct, 0, 0 ) );
	RENDER
		->begin()
		->push_color( W_COLOR_TEAL )
		->draw_sprite( engine->white_solid )
		->end();
	OPENGL
		->pop();

	// ----------------------------------------------------------------------------

	step_per_fts = tween_rotate->step_per_sec / w_time::FTS_desired_frames_per_second;

	OPENGL
		->push( true )
		->add_transform( rotate_xform )
		->rotate( step_per_fts * RENDER->frame_interpolate_pct );
	RENDER
		->begin()
		->push_color( W_COLOR_ORANGE )
		->draw_sprite( engine->white_solid )
		->end();
	OPENGL
		->pop();
	*/
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
					tile_from_screen_pos( engine->input_mgr->mouse_vwindow_pos.x, engine->input_mgr->mouse_vwindow_pos.y );
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
					hover_tile.x--;
					hover_tile.x = w_clamp( hover_tile.x, 0, 18 );
				}
				break;

				case e_input_id::controller_button_dpad_right:
				case e_input_id::keyboard_right:
				{
					hover_tile.x++;
					hover_tile.x = w_clamp( hover_tile.x, 0, 18 );
				}
				break;

				case e_input_id::controller_button_dpad_up:
				case e_input_id::keyboard_up:
				{
					hover_tile.y--;
					hover_tile.y = w_clamp( hover_tile.y, 0, 8 );
				}
				break;

				case e_input_id::controller_button_dpad_down:
				case e_input_id::keyboard_down:
				{
					hover_tile.y++;
					hover_tile.y = w_clamp( hover_tile.y, 0, 8 );
				}
				break;

				case e_input_id::keyboard_0:
				{
					game->current_room = 0;
				}
				break;
				case e_input_id::keyboard_1:
				{
					game->current_room = 1;
				}
				break;
				case e_input_id::keyboard_2:
				{
					game->current_room = 2;
				}
				break;
				case e_input_id::keyboard_3:
				{
					game->current_room = 3;
				}
				break;
				case e_input_id::keyboard_4:
				{
					game->current_room = 4;
				}
				break;
				case e_input_id::keyboard_5:
				{
					game->current_room = 5;
				}
				break;
				case e_input_id::keyboard_6:
				{
					game->current_room = 6;
				}
				break;
				case e_input_id::keyboard_7:
				{
					game->current_room = 7;
				}
				break;
				case e_input_id::keyboard_8:
				{
					game->current_room = 8;
				}
				break;
				case e_input_id::keyboard_9:
				{
					game->current_room = 9;
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

	draw_selector_bracket = ( tile_x >= 0 && tile_x < 19 && tile_y >= 0 && tile_y < 9 );
	hover_tile = w_vec2( round( tile_x ), round( tile_y ) );
}
