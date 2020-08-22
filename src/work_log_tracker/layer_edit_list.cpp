
#include "app_header.h"

void layer_edit_list::push()
{
	test_entity = GAME->spawn_entity<w_entity>( { v_window_hw, 0.0f } );

	test_entity->add_component<ec_emitter>()
		->init( "background_fire_down" );

	test_entity->add_component<ec_emitter>()
		->init( "background_fire_up" )
		->set_transform( { 0.0f, v_window_h }, 0.0f, 1.0f );

	GAME->load_data();
}

void layer_edit_list::draw()
{
	RENDER->begin();
	GAME->draw_entities();
	RENDER->end();

	if( !style_button )
	{
		style_button = std::make_unique<w_ui_style_button>();

		style_approved_button = std::make_unique<w_ui_style_button>();
		style_approved_button->color_slice_def = w_color::dark_green;
		style_approved_button->color_label = w_color::yellow;

		style_clear_button = std::make_unique<w_ui_style_button>();
		style_clear_button->slice_def = nullptr;
		style_clear_button->subtex = engine->get_asset<a_subtexture>("gear_icon");
		style_clear_button->subtex_sz = w_sz( 24, 24 );
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
			->push_depth_nudge( 100 );

		w_ui_style* style = artist.approved ? style_approved_button.get() : style_button.get();
		if( UI->im_active( artist.name, w_rect( 8, ypos, 130, 16 ), *( style ) ) & im_result::left_clicked )
		{
			artist.approved = !artist.approved;
			GAME->save_data();
		}
		RENDER->end();

		ypos += 18.0f;
	}
}

bool layer_edit_list::handle_input_event( const w_input_event* evt )
{
	//if( evt->event_id == event_id::input_pressed && evt->input_id == input_id::key_space )
	//{
	//	test_entity->pos = engine->input->mouse_vwindow_pos;
	//	return true;
	//}

	return false;
}
