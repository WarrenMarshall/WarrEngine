
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );

	engine->ui_mgr->set_mouse_visible( true );

	tile_display_area.min = { 0, 32 };
	tile_display_area.max = { v_window_w, 32 + ( TILE_SZ * 9 ) };

	rc_button_browse = w_rect( 12, 188, 48, 48 );
}

void layer_editor::pop()
{
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

	// current room

	RENDER
		->begin()
		->push_depth( 100.0f )
		->draw_string( engine->ui_mgr->ui_font, s_format( "Current Room: %d", game->current_room ),
			w_rect( 68.0f, 206.0f ) )
		->end();

	// current tile

	RENDER
		->begin()
		->push_depth( 100.0f )
		->draw_sprite( game->get_tile( game->current_tile_idx )->subtexture, w_rect( 36,212, 32,32 ) )
		->end();

	// ----------------------------------------------------------------------------
	// frames

	// title bar

	RENDER->begin()
		->push_depth( 50.0f )
		->push_color( W_COLOR_DARK_GREY )
		->draw_sliced( panel_slice_def, w_rect( 0.0f, 0.0f, v_window_w, static_cast<float>( TILE_SZ ) * 2.0f ) )
		->end();

	game->draw_viewport_caption();

	// info bars

	RENDER->begin()
		->push_color( W_COLOR_DARK_GREY )
		->draw_sliced( panel_slice_def, w_rect( 0.0f, v_window_h - 68.0f, v_window_w, 68.0f ) )
		->end();

	// ----------------------------------------------------------------------------
	// tiles

	float ypos = TILE_SZ * 2;

	for( int y = 0 ; y < ROOM_H ; ++y )
	{
		e_ui_id id = e_ui_id::tile_start;
		for( int x = 0 ; x < ROOM_W ; ++x )
		{
			int idx = ( y * ROOM_W ) + x;

			w_rect rc = w_rect(
				x * (float)TILE_SZ, ypos + (y * (float) TILE_SZ),
				TILE_SZ - 1, TILE_SZ - 1
			);

			//RENDER->draw( game->get_tile( game->rooms[ game->current_room ].tiles[ idx ] )->subtexture, rc );

			if( engine->ui_mgr->im_image( id, game->get_tile( game->rooms[ game->current_room ].tiles[ idx ] )->subtexture, rc ) )
			{
				log_msg( "clicked tile!" );
			}
		}
	}

	// ----------------------------------------------------------------------------

 	if( engine->ui_mgr->im_button( e_ui_id::browse, panel_slice_def, rc_button_browse ) )
 	{
 		log_msg( "BUTTON CLICKED!" );
	}
}
	
void layer_editor::handle_input_event( const w_input_event* evt )
{
	switch( evt->event_id )
	{
		case e_event_id::input_motion:
		{
			switch( evt->input_id )
			{
				case e_input_id::mouse:
				{
					/*
					if( c2CircletoAABB( engine->input_mgr->c2_mouse_vpos, tile_display_area ) )
					{
						set_current_tile_from_mouse_pos( engine->input_mgr->mouse_vwindow_pos.x, engine->input_mgr->mouse_vwindow_pos.y );

						if( is_painting )
						{
							paint_current_tile();
						}
					}
					*/
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
				{
					if( c2AABBtoPoint( tile_display_area, engine->input_mgr->mouse_vwindow_pos ) )
					{
						is_painting = true;
						paint_current_tile();
					}
				}
				break;

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
