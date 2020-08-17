
#include "app_header.h"

void layer_editor::push()
{
	UI->set_mouse_visible( true );

	tile_display_area = w_rect( 0, 32, v_window_w, TILE_SZ * ROOM_SZ );
	gradient = engine->get_asset<a_gradient>( "background_gradient" );
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
#if 0
	// outside the bounds, stop filling
	if( x >= ROOM_SZ
		|| x < 0
		|| y >= ROOM_SZ
		|| y < 0 )
	{
		return;
	}

	int idx = ( y * ROOM_SZ ) + x;

	if( GAME->rooms[ GAME->current_room_idx ].tiles[ idx ] == old_tile_id )
	{
		GAME->rooms[ GAME->current_room_idx ].tiles[ idx ] = new_tile_id;

		flood_fill_room_worker( x + 1, y, old_tile_id, new_tile_id );
		flood_fill_room_worker( x - 1, y, old_tile_id, new_tile_id );
		flood_fill_room_worker( x, y + 1, old_tile_id, new_tile_id );
		flood_fill_room_worker( x, y - 1, old_tile_id, new_tile_id );
	}
#endif
}

void flood_fill_room( int idx_clicked, int old_tile_id, int new_tile_id )
{
#if 0
	if( old_tile_id == new_tile_id )
	{
		return;
	}

	int y = idx_clicked / ROOM_SZ;
	int x = idx_clicked - ( y * ROOM_SZ );

	flood_fill_room_worker( x, y, old_tile_id, new_tile_id );
#endif
}

void layer_editor::draw()
{
	if( !style_browse  )
	{
		style_browse = std::make_unique<w_ui_style_button>();
		style_browse->subtex_sz = w_sz( 32, 32 );

		style_arrow_button = std::make_unique<w_ui_style_button>();
		style_arrow_button->subtex_pos_offset = w_vec2( -4, -4 );
		style_arrow_button->subtex_sz = w_sz( 8, 8 );

		style_panel = std::make_unique<w_ui_style_panel>();
		style_panel->color = w_color::dark_grey;

		style_tile = std::make_unique<w_ui_style_tile>();
	}

	w_layer::draw();

	// current room

	RENDER
		->begin()
		->push_depth_nudge()
		->draw_string( UI->theme->small_font, "Room", w_rect( 88.0f, 192.0f ) )
		->push_align( align::centered )
		->push_depth_nudge()
		->push_rgb( w_color::teal )
		->draw_string( UI->theme->large_font, fmt::format( "{}", GAME->current_room_idx ), w_rect( 102.0f, 218.0f ) )
		->end();

	// title bar

	UI->im_passive( { 0.0f, 0.0f, v_window_w, static_cast<float>( TILE_SZ ) * 2.0f }, *( style_panel.get() ) );
	GAME->draw_viewport_caption( "Endless Adventure Editor", 18.0f);

	// info bars

	UI->im_passive( { 0.0f, v_window_h - 68.0f, v_window_w, 68.0f }, *( style_panel.get() ) );

	// tiles

	bool c_key_is_down = engine->input->is_button_down( input_id::key_c );
	bool f_key_is_down = engine->input->is_button_down( input_id::key_f );

	float xpos = (v_window_w - ( ROOM_SZ * TILE_SZ )) / 2.0f;
	float ypos = TILE_SZ * 2;

	RENDER
		->begin()
		->push_alpha( 0.25f )
		->draw( gradient, w_rect( 0, ypos, v_window_w, ROOM_SZ * TILE_SZ ) )
		->end();

	for( int y = 0 ; y < ROOM_SZ ; ++y )
	{
		for( int x = 0 ; x < ROOM_SZ ; ++x )
		{
			int idx = ( y * ROOM_SZ ) + x;

			w_rect rc = w_rect(
				xpos + (x * (float)TILE_SZ), ypos + (y * (float) TILE_SZ),
				TILE_SZ, TILE_SZ
			);

			style_tile->subtex_tile = GAME->get_tile( GAME->rooms[ GAME->current_room_idx ].tile_ids[ room_layer::geometry ][ idx ] )->subtex;
			e_im_result ir = UI->im_active( "", rc, *( style_tile.get() ) );

			if( ir & im_result::hot )
			{
				GAME->rooms[ GAME->current_room_idx ].tile_ids[ room_layer::geometry ][ idx ] = GAME->current_tile_idx;
				is_painting = true;
			}
			else if( ir & im_result::hovered )
			{
				if( is_painting )
				{
					GAME->rooms[ GAME->current_room_idx ].tile_ids[ room_layer::geometry ][ idx ] = GAME->current_tile_idx;
				}
				else if( c_key_is_down )
				{
					// copy tile clicked to the current tile
					GAME->current_tile_idx = GAME->rooms[ GAME->current_room_idx ].tile_ids[ room_layer::geometry ][ idx ];
				}
				else if( f_key_is_down )
				{
					// take the current tile, and flood fill into the room starting with the clicked tile
					flood_fill_room( idx, GAME->rooms[ GAME->current_room_idx ].tile_ids[ room_layer::geometry ][ idx ], GAME->current_tile_idx );
				}
			}
		}
	}

	style_browse->subtex = GAME->get_tile( GAME->current_tile_idx )->subtex;
	if( UI->im_active( "", { 12, 188, 48, 48 }, *( style_browse.get() ) ) & im_result::left_clicked )
	{
		engine->layer_mgr->push( std::make_shared<layer_browser>() );
	}
	
	style_arrow_button->subtex = engine->get_asset<a_subtexture>( "ui_arrow_left" );
	if( UI->im_active( "", { 76.0f, 208.0f, 16, 16 }, *( style_arrow_button.get() ) ) & im_result::left_clicked )
	{
		GAME->current_room_idx--;
		GAME->current_room_idx = w_clamp( GAME->current_room_idx, 0, 9 );
	}

	style_arrow_button->subtex = engine->get_asset<a_subtexture>( "ui_arrow_right" );
	if( UI->im_active( "", { 114.0f, 208.0f, 16, 16 }, *( style_arrow_button.get() ) ) & im_result::left_clicked )
	{
		GAME->current_room_idx++;
		GAME->current_room_idx = w_clamp( GAME->current_room_idx, 0, 9 );
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
					GAME->current_room_idx++;
					GAME->current_room_idx = w_clamp( GAME->current_room_idx, 0, 9 );
				}
				break;

				case input_id::key_left_bracket:
				{
					GAME->current_room_idx--;
					GAME->current_room_idx = w_clamp( GAME->current_room_idx, 0, 9 );
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
					GAME->current_room_idx = evt->input_id - input_id::key_0;
					return true;
				}
				break;
			}
		}
		break;
	}

	return false;
}