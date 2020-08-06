
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_editor::push()
{
	UI->set_mouse_visible( true );

	tile_display_area = w_rect( 0, 32, v_window_w, TILE_SZ * ROOM_SZ );
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
	if( x >= ROOM_SZ
		|| x < 0
		|| y >= ROOM_SZ
		|| y < 0 )
	{
		return;
	}

	int idx = ( y * ROOM_SZ ) + x;

	if( game->geometry_layer[ game->current_room_idx ].tiles[ idx ] == old_tile_id )
	{
		game->geometry_layer[ game->current_room_idx ].tiles[ idx ] = new_tile_id;

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

	int y = idx_clicked / ROOM_SZ;
	int x = idx_clicked - ( y * ROOM_SZ );

	flood_fill_room_worker( x, y, old_tile_id, new_tile_id );
}

void layer_editor::draw()
{
	w_layer::draw();

	// current room

	RENDER
		->begin()
		->push_depth_nudge()
		->draw_string( UI->theme->small_font, "Room", w_rect( 88.0f, 192.0f ) )
		->push_align( align::centered )
		->push_depth_nudge()
		->push_rgb( W_COLOR_TEAL )
		->draw_string( UI->theme->large_font, fmt::format( "{}", game->current_room_idx ), w_rect( 102.0f, 218.0f ) )
		->end();

	// title bar

	UI->im_passive( this, { 0.0f, 0.0f, v_window_w, static_cast<float>( TILE_SZ ) * 2.0f }, w_ui_style_panel( UI->theme->panel_slice_def) );
	game->draw_viewport_caption( "Endless Adventure Editor", 18.0f);

	// info bars

	UI->im_passive( this, { 0.0f, v_window_h - 68.0f, v_window_w, 68.0f }, w_ui_style_panel( UI->theme->panel_slice_def ) );

	// tiles

	bool c_key_is_down = engine->input->is_button_down( input_id::key_c );
	bool f_key_is_down = engine->input->is_button_down( input_id::key_f );

	float xpos = (v_window_w - ( ROOM_SZ * TILE_SZ )) / 2.0f;
	float ypos = TILE_SZ * 2;

	for( int y = 0 ; y < ROOM_SZ ; ++y )
	{
		for( int x = 0 ; x < ROOM_SZ ; ++x )
		{
			int idx = ( y * ROOM_SZ ) + x;

			w_rect rc = w_rect(
				xpos + (x * (float)TILE_SZ), ypos + (y * (float) TILE_SZ),
				TILE_SZ, TILE_SZ
			);

			a_subtexture* subtex = game->get_tile( game->geometry_layer[ game->current_room_idx ].tiles[ idx ] )->subtex;
			
			e_im_result ir = UI->im_active( this, rc, w_ui_style_tile( subtex ) );

			if( ir & im_result::hot )
			{
				game->geometry_layer[ game->current_room_idx ].tiles[ idx ] = game->current_tile_idx;
				is_painting = true;
			}
			else if( ir & im_result::hovered )
			{
				if( is_painting )
				{
					game->geometry_layer[ game->current_room_idx ].tiles[ idx ] = game->current_tile_idx;
				}
				else if( c_key_is_down )
				{
					// copy tile clicked to the current tile
					game->current_tile_idx = game->geometry_layer[ game->current_room_idx ].tiles[ idx ];
				}
				else if( f_key_is_down )
				{
					// take the current tile, and flood fill into the room starting with the clicked tile
					flood_fill_room( idx, game->geometry_layer[ game->current_room_idx ].tiles[ idx ], game->current_tile_idx );
				}
			}
		}
	}

	if( UI->im_active( this, { 12, 188, 48, 48 }, w_ui_style_pushbutton( UI->theme->button_slice_def, game->get_tile( game->current_tile_idx )->subtex ) ) & im_result::left_clicked )
	{
		engine->layer_mgr->push( std::make_unique<layer_browser>() );
	}
	if( UI->im_active( this, { 76.0f, 208.0f, 16, 16 }, w_ui_style_pushbutton( UI->theme->button_slice_def, engine->get_asset<a_subtexture>( "ui_arrow_left" ) ) ) & im_result::left_clicked )
	{
		game->current_room_idx--;
		game->current_room_idx = w_clamp( game->current_room_idx, 0, 9 );
	}
	if( UI->im_active( this, { 114.0f, 208.0f, 16, 16 }, w_ui_style_pushbutton( UI->theme->button_slice_def, engine->get_asset<a_subtexture>( "ui_arrow_right" ) ) ) & im_result::left_clicked )
	{
		game->current_room_idx++;
		game->current_room_idx = w_clamp( game->current_room_idx, 0, 9 );
	}

	RENDER
		->begin()
		->push_depth_nudge()
		->push_align( align::vcenter )
		->draw_string( UI->theme->small_font, "Geometry", w_rect( 164.0f, 202.0f ) )
		->draw_string( UI->theme->small_font, "Items", w_rect( 164.0f, 222.0f ) )
		->end();

	static a_subtexture* checkmark = engine->get_asset<a_subtexture>( "ui_checkmark" );
	if( UI->im_active( this, { 140.0f, 190.0f, 24, 24 }, w_ui_style_radiobutton( nullptr, engine->get_asset<a_subtexture>( "ui_radio" ), checkmark ) ) & im_result::left_clicked )
	{
	}
	if( UI->im_active( this, { 140.0f, 210.0f, 24, 24 }, w_ui_style_radiobutton( nullptr, engine->get_asset<a_subtexture>( "ui_radio" ), checkmark ) ) & im_result::left_clicked )
	{
	}

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
				case input_id::key_right_bracket:
				{
					game->current_room_idx++;
					game->current_room_idx = w_clamp( game->current_room_idx, 0, 9 );
				}
				break;

				case input_id::key_left_bracket:
				{
					game->current_room_idx--;
					game->current_room_idx = w_clamp( game->current_room_idx, 0, 9 );
				}
				break;

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
					game->current_room_idx = evt->input_id - input_id::key_0;
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
	auto idx = static_cast<int>( ( game->current_tile.y * ROOM_SZ ) + game->current_tile.x );
	game->geometry_layer[ game->current_room_idx ].tiles[ idx ] = game->current_tile_idx;
}

void layer_editor::set_current_tile_idx_from_current_tile()
{
	auto idx = static_cast<int>( ( game->current_tile.y * ROOM_SZ ) + game->current_tile.x );
	game->current_tile_idx = game->geometry_layer[ game->current_room_idx ].tiles[ idx ];
}
