
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
	game->viewport_caption = "Endless Adventure Editor";
}

void layer_editor::draw()
{
	w_layer::draw();

	// ----------------------------------------------------------------------------

	MATRIX
		->push_identity()
		->translate( w_vec2( -v_window_hw, v_window_hh - ( TILE_SZ * 3 ) ) )
		->translate( w_vec2( ( game->current_tile.x * TILE_SZ ), -( game->current_tile.y * TILE_SZ ) ) );

	RENDER
		->begin()
		->push_depth( 10.f )
		->draw( selector_bracket )
		->end();

	MATRIX->pop();

	// ----------------------------------------------------------------------------

	MATRIX
		->push_identity()
		->translate( { -84.0f, -68.0f } );
	RENDER
		->begin()
		->push_depth( 100.0f )
		->draw_string( engine->ui_mgr->ui_font, s_format( "Current Room: %d", game->current_room ) )
		->end();

	MATRIX->top()->translate( { -32, -22 } );
	RENDER
		->begin()
		->push_depth( 100.0f )
		->draw_sprite( game->get_tile( game->current_tile_idx )->img, { 32,32 } )
		->end();

	MATRIX->pop();

	// ----------------------------------------------------------------------------
	// frames

	MATRIX->push_identity()
		->translate( { -v_window_hw, v_window_hh } );
	{
		// title bar

		RENDER->begin()
			->push_depth( 50.0f )
			->push_color( W_COLOR_DARK_GREY )
			->draw_sliced( panel_slice_def, { v_window_w, TILE_SZ*2.0f } )
			->end();

		game->draw_viewport_caption();

		// info bars

		MATRIX->push()->translate( { 0.0f, -(TILE_SZ * 11.0f) } );
		RENDER->begin()
			->push_color( W_COLOR_DARK_GREY )
			->draw_sliced( panel_slice_def, { v_window_w, 68.0f } )
			->end();

		MATRIX->top()->translate( { 12, -12 } );
		RENDER->begin()
			->push_color( W_COLOR_DARK_GREY )
			->push_depth( 50.f )
			->draw_sliced( panel_slice_def, { 48.0f, 48.0f } )
			->end();
		MATRIX->pop();
	}
	MATRIX->pop();

	MATRIX->push();
	game->draw_entities();
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
						set_current_tile_from_mouse_pos( engine->input_mgr->mouse_vwindow_pos.x, engine->input_mgr->mouse_vwindow_pos.y );

						if( is_painting )
						{
							paint_current_tile();
						}
					}
				}
				break;
			}
		}
		break;

		case e_event_id::input_released:
		{
			switch( evt->input_id )
			{
				case e_input_id::mouse_button_left:
				case e_input_id::keyboard_space:
				{
					is_painting = false;
				}
				break;
			}
		}
		break;

		case e_event_id::input_pressed:
		{
			switch( evt->input_id )
			{
				case e_input_id::mouse_button_left:
				case e_input_id::keyboard_space:
				{
					is_painting = true;
					paint_current_tile();
				}
				break;

				case e_input_id::mouse_button_right:
				{
					set_current_tile_idx_from_current_tile();
				}
				break;

				case e_input_id::keyboard_c:
				{
					if( evt->ctrl_down )
					{
						set_current_tile_idx_from_current_tile();
					}
				}
				break;

				case e_input_id::keyboard_left:
				{
					if( evt->alt_down )
					{
						game->current_room--;
						game->current_room = w_max( game->current_room, 0 );
					}
					else
					{
						game->current_tile.x--;
						game->current_tile.x = w_max( game->current_tile.x, 0 );
					}
				}
				break;

				case e_input_id::keyboard_right:
				{
					if( evt->alt_down )
					{
						game->current_room++;
						game->current_room = w_min( game->current_room, static_cast<int>( game->rooms.size() - 1 ) );
					}
					else
					{
						game->current_tile.x++;
						game->current_tile.x = w_min( game->current_tile.x, 18 );
					}
				}
				break;

				case e_input_id::keyboard_up:
				{
					game->current_tile.y--;
					game->current_tile.y = w_max( game->current_tile.y, 0 );
				}
				break;

				case e_input_id::keyboard_down:
				{
					game->current_tile.y++;
					game->current_tile.y = w_min( game->current_tile.y, 8 );
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

void layer_editor::set_current_tile_from_mouse_pos( float xpos, float ypos )
{
	float tiles_x = TILE_SZ / 2;
	float tiles_y = TILE_SZ * 2.5;

	float tile_x = round(( xpos - tiles_x ) / (float)TILE_SZ);
	float tile_y = round(( ypos - tiles_y ) / (float)TILE_SZ);

	game->current_tile = w_vec2( round( tile_x ), round( tile_y ) );
	game->current_tile.x = w_clamp( game->current_tile.x, 0, 18 );
	game->current_tile.y = w_clamp( game->current_tile.y, 0, 8 );
}

void layer_editor::paint_current_tile()
{
	int idx = static_cast<int>( ( game->current_tile.y * ROOM_W ) + game->current_tile.x );
	game->rooms[ game->current_room ].tiles[ idx ] = game->current_tile_idx;
}

void layer_editor::set_current_tile_idx_from_current_tile()
{
	int idx = static_cast<int>( ( game->current_tile.y * ROOM_W ) + game->current_tile.x );
	game->current_tile_idx = game->rooms[ game->current_room ].tiles[ idx ];
}
