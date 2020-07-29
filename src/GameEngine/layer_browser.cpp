
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
	game->viewport_caption = "Select A Tile";
}

void layer_browser::draw()
{
	w_layer::draw();

	UI->im_passive( { 16.0f, 0.0f, v_window_w - 32.0f, v_window_h - 32.0f }, w_ui_style_panel( UI->theme->panel_slice_def ) );
	game->draw_viewport_caption();

	if( UI->im_active( { 24.0f, 16.0f, 24, 24 }, w_ui_style_pushbutton( UI->theme->button_slice_def, nullptr ) ) & im_result::left_clicked )
	{
		engine->layer_mgr->pop();
	}
}
