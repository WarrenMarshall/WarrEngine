
#include "master_pch.h"
#include "master_header.h"

// ------------------------------------------------------------------------------

void layer_browser::push()
{
	grad_overlay = engine->get_asset<a_gradient>( "background_gradient" );
}

void layer_browser::pop()
{
}

void layer_browser::becoming_top_layer()
{
}

void layer_browser::draw()
{
	if( !style_panel )
	{
		style_close_button = std::make_unique<w_ui_style_pushbutton>();
		style_close_button->subtex = engine->get_asset<a_subtexture>( "ui_close_x" );
		style_close_button->subtex_offset = w_vec2( -4, -4 );
		style_close_button->subtex_sz = w_sz( 8, 8 );

		style_panel = std::make_unique<w_ui_style_panel>();
		style_panel->color = W_COLOR_DARK_GREY;

		style_tile = std::make_unique<w_ui_style_tile>();
	}

	w_layer::draw();

	UI->im_passive( this, { 16.0f, 16.0f, v_window_w - 32.0f, v_window_h - 32.0f }, *( style_panel.get() ) );
	game->draw_viewport_caption( "Select A Tile", 34.0f );

	if( UI->im_active( this, { 24.0f, 24.0f, 16, 16 }, *( style_close_button.get() ) ) & im_result::left_clicked )
	{
		engine->layer_mgr->pop();
	}

	w_vec2 draw_pos( 24, 54 );
	int tile_draw_count = 0;
	int tile_idx = 0;

	for( auto& tile : game->tile_masters )
	{
		if( tile.show_in_browser )
		{
			w_rect rc( draw_pos.x, draw_pos.y, TILE_SZ, TILE_SZ );
			style_tile->subtex_tile = tile.subtex;
			e_im_result ir = UI->im_active( this, rc, *( style_tile.get() ) );

			if( ir == im_result::left_clicked )
			{
				game->current_tile_idx = tile_idx;
				engine->layer_mgr->pop();
			}

			draw_pos.x += TILE_SZ * 1.1f;

			tile_draw_count++;
			if( !(tile_draw_count % 15) )
			{
				draw_pos.x = 24;
				draw_pos.y += TILE_SZ * 1.1f;
			}
		}

		tile_idx++;
	}
}
