
#include "app_header.h"

void layer_edit_list::push()
{
	GAME->load_data();
}

void layer_edit_list::draw()
{
	w_layer::draw();

	if( !style_button )
	{
		style_button = std::make_unique<w_ui_style_button>();

		style_approved_button = std::make_unique<w_ui_style_button>();
		style_approved_button->base_attrib.color = w_color::dark_green;
		style_approved_button->label_attrib.color = w_color::yellow;

		style_waiting_button = std::make_unique<w_ui_style_button>();
		style_waiting_button->base_attrib.color = w_color::dark_grey;
		style_waiting_button->label_attrib.color = w_color::white;

		style_clear_button = std::make_unique<w_ui_style_button>();
		style_clear_button->subtex = engine->get_asset<a_subtexture>("gear_icon");
		style_clear_button->subtex_attrib.sz = w_sz( 24, 24 );
		style_clear_button->slice_def = std::nullopt;
	}

	RENDER
		->begin()
		->push_depth_nudge( 100 );
	if( UI->im_active( "", w_rect( v_window_w - 24, 6, 24, 24) , *( style_clear_button.get() ) ) & im_result::left_clicked )
	{
		for( auto& artist : GAME->artists )
		{
			artist.approved = false;
		}
		GAME->save_data();
	}
	RENDER->end();

	float ypos = 6.0f;

	for( auto& artist : GAME->artists )
	{
		RENDER
			->begin()
			->push_depth_nudge( 10 );

		w_ui_style* style = artist.approved ? style_approved_button.get() : style_waiting_button.get();
		if( UI->im_active( artist.name, w_rect( 8, ypos, 130, 16 ), *( style ) ) & im_result::left_clicked )
		{
			artist.approved = !artist.approved;
			GAME->save_data();
		}
		RENDER->end();

		ypos += 18.0f;
	}
}
