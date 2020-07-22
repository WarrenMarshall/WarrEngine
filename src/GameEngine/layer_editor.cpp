
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
	rc_button_browse2 = w_rect( 80, 188, 32, 32 );
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
				TILE_SZ, TILE_SZ
			);

			//if( engine->ui_mgr->im_image( id, game->get_tile( game->rooms[ game->current_room ].tiles[ idx ] )->subtexture, W_COLOR_LIGHT_GREY, rc ) )
			//{
			//	log_msg( "clicked tile!" );
			//}	
		}
	}

	// ----------------------------------------------------------------------------

	//engine->ui_mgr->hover_id = -1;
	if( engine->ui_mgr->im_button( 667, panel_slice_def, W_COLOR_DARK_GREY, rc_button_browse2 ) )
	{
		log_msg( "BUTTON 2 CLICKED!" );
	}
	//engine->ui_mgr->hover_id = -1;
 	if( engine->ui_mgr->im_button( 666, panel_slice_def, W_COLOR_DARK_GREY, rc_button_browse ) )
 	{
 		log_msg( "BUTTON CLICKED!" );
	}

	RENDER
		->begin()
		->push_depth( 1000 )
		->draw_string(
			engine->ui_mgr->ui_font,
			s_format( "%d / %d", engine->ui_mgr->hover_id, engine->ui_mgr->clicked_id ),
			w_rect( 0, 0 ) )
		->end();
}
	
bool layer_editor::handle_input_event( const w_input_event* evt )
{
	switch( evt->event_id )
	{
		case e_event_id::input_released:
		{
			switch( evt->input_id )
			{
				case e_input_id::mouse_button_left:
				{
					is_painting = false;
					return true;
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
						return true;
					}
				}
				break;

				case e_input_id::key_space:
				{
					is_painting = true;
					paint_current_tile();
					return true;
				}
				break;

				case e_input_id::mouse_button_right:
				{
					set_current_tile_idx_from_current_tile();
					return true;
				}
				break;

				case e_input_id::key_left:
				{
					game->current_room--;
					game->current_room = w_max( game->current_room, 0 );
					return true;
				}
				break;

				case e_input_id::key_right:
				{
					game->current_room++;
					game->current_room = w_min( game->current_room, static_cast<int>( game->rooms.size() - 1 ) );
					return true;
				}
				break;

				case e_input_id::key_0:
				case e_input_id::key_1:
				case e_input_id::key_2:
				case e_input_id::key_3:
				case e_input_id::key_4:
				case e_input_id::key_5:
				case e_input_id::key_6:
				case e_input_id::key_7:
				case e_input_id::key_8:
				case e_input_id::key_9:
				{
					game->current_room = static_cast<int>( evt->input_id ) - static_cast<int>( e_input_id::key_0 );
					return true;
				}
				break;
			}
		}
		break;
	}

	return false;
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
