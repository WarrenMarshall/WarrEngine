
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	UI->set_mouse_visible( true );

	tile_display_area = w_rect( 0, 32, v_window_w, TILE_SZ * 9 );
}

void layer_editor::pop()
{
	UI->set_mouse_visible( false );
}

void layer_editor::becoming_top_layer()
{
}

void flood_fill_room_worker( int x, int y, int old_tile_id, int new_tile_id )
{
	// outside the bounds, stop filling
	if( x >= 19
		|| x < 0
		|| y >= 9
		|| y < 0 )
	{
		return;
	}

	int idx = ( y * 19 ) + x;

	if( game->rooms[ game->current_room ].tiles[ idx ] == old_tile_id )
	{
		game->rooms[ game->current_room ].tiles[ idx ] = new_tile_id;

		flood_fill_room_worker( x + 1, y, old_tile_id, new_tile_id );
		flood_fill_room_worker( x - 1, y, old_tile_id, new_tile_id );
		flood_fill_room_worker( x, y + 1, old_tile_id, new_tile_id );
		flood_fill_room_worker( x, y - 1, old_tile_id, new_tile_id );
	}
}

void flood_fill_room( int idx_clicked, int old_tile_id, int new_tile_id )
{
	if( old_tile_id == new_tile_id )
	{
		return;
	}

	int y = idx_clicked / 19;
	int x = idx_clicked - ( y * 19 );

	flood_fill_room_worker( x, y, old_tile_id, new_tile_id );
}

void layer_editor::draw()
{
	w_layer::draw();

	// ----------------------------------------------------------------------------

	// current room

	RENDER
		->begin()
		->push_depth_nudge()
		->draw_string( UI->theme->small_font, fmt::format( "Current Room: {}", game->current_room ),
			w_rect( 68.0f, 206.0f ) )
		->end();

	// title bar

	UI->im_passive( this, { 0.0f, 0.0f, v_window_w, static_cast<float>( TILE_SZ ) * 2.0f }, w_ui_style_panel( UI->theme->panel_slice_def) );
	game->draw_viewport_caption( "Endless Adventure Editor", 18.0f);

	// info bars

	UI->im_passive( this, { 0.0f, v_window_h - 68.0f, v_window_w, 68.0f }, w_ui_style_panel( UI->theme->panel_slice_def ) );

	// ----------------------------------------------------------------------------
	// tiles

	bool c_key_is_down = engine->input->is_button_down( input_id::key_c );
	bool f_key_is_down = engine->input->is_button_down( input_id::key_f );

	float ypos = TILE_SZ * 2;

	for( int y = 0 ; y < ROOM_H ; ++y )
	{
		for( int x = 0 ; x < ROOM_W ; ++x )
		{
			int idx = ( y * ROOM_W ) + x;

			w_rect rc = w_rect(
				x * (float)TILE_SZ, ypos + (y * (float) TILE_SZ),
				TILE_SZ, TILE_SZ
			);

			a_subtexture* subtex = game->get_tile( game->rooms[ game->current_room ].tiles[ idx ] )->subtex;
			
			e_im_result ir = UI->im_active( this, rc, w_ui_style_tile( subtex ) );

			if( (ir & im_result::left_clicked) && !f_key_is_down )
			{
				// by default, just drop down the current 
				game->rooms[ game->current_room ].tiles[ idx ] = game->current_tile_idx;
			}
			else if( ir & im_result::hot )
			{
				if( c_key_is_down )
				{
					// copy tile clicked to the current tile
					game->current_tile_idx = game->rooms[ game->current_room ].tiles[ idx ];
				}
				else if( f_key_is_down )
				{
					// take the current tile, and flood fill into the room starting with the clicked tile
					flood_fill_room( idx, game->rooms[ game->current_room ].tiles[ idx ], game->current_tile_idx );
				}
				else
				{
					game->rooms[ game->current_room ].tiles[ idx ] = game->current_tile_idx;
					is_painting = true;
				}
			}
			else if( ( ir & im_result::hovered ) && is_painting )
			{
				game->rooms[ game->current_room ].tiles[ idx ] = game->current_tile_idx;
			}
		}
	}

	// ----------------------------------------------------------------------------

	if( UI->im_active( this, { 12, 188, 48, 48 }, w_ui_style_pushbutton( UI->theme->button_slice_def, game->get_tile( game->current_tile_idx )->subtex ) ) & im_result::left_clicked )
	{
		engine->layer_mgr->push( std::make_unique<layer_browser>() );
	}

	// ----------------------------------------------------------------------------

	RENDER
		->begin()
		->push_depth_nudge( 100 );
	game->draw_entities();
	RENDER->end();
}
	
bool layer_editor::handle_input_event( const w_input_event* evt )
{
	switch( evt->event_id )
	{
		case event_id::input_released:
		{
			switch( evt->input_id )
			{
				case input_id::mouse_button_left:
				{
					is_painting = false;
					return true;
				}
				break;
			}
		}
		break;

		case event_id::input_pressed:
		{
			switch( evt->input_id )
			{
				case input_id::key_0:
				case input_id::key_1:
				case input_id::key_2:
				case input_id::key_3:
				case input_id::key_4:
				case input_id::key_5:
				case input_id::key_6:
				case input_id::key_7:
				case input_id::key_8:
				case input_id::key_9:
				{
					game->current_room = evt->input_id - input_id::key_0;
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
	game->current_tile.x = w_clamp( game->current_tile.x, 0.f, 18.f );
	game->current_tile.y = w_clamp( game->current_tile.y, 0.f, 8.f );
}

void layer_editor::paint_current_tile()
{
	auto idx = static_cast<int>( ( game->current_tile.y * ROOM_W ) + game->current_tile.x );
	game->rooms[ game->current_room ].tiles[ idx ] = game->current_tile_idx;
}

void layer_editor::set_current_tile_idx_from_current_tile()
{
	auto idx = static_cast<int>( ( game->current_tile.y * ROOM_W ) + game->current_tile.x );
	game->current_tile_idx = game->rooms[ game->current_room ].tiles[ idx ];
}
