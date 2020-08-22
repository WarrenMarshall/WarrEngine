
#include "app_header.h"

void layer_edit_list::push()
{
	test_entity = GAME->spawn_entity<w_entity>( { v_window_hw, 0.0f } );

	test_entity->add_component<ec_emitter>()
		->init( "background_fire_down" );

	test_entity->add_component<ec_emitter>()
		->init( "background_fire_up" )
		->set_transform( { 0.0f, v_window_h }, 0.0f, 1.0f );
}

void layer_edit_list::draw()
{
	RENDER->begin();
	GAME->draw_entities();
	RENDER->end();

	if( !style_button )
	{
		style_button = std::make_unique<w_ui_style_button>();
	}

	float ypos = 6.0f;

	for( auto& iter : GAME->artists )
	{
		RENDER
			->begin()
			->push_depth_nudge( 100 );
		if( UI->im_active( iter.name, w_rect( 8, ypos, 130, 16 ), *( style_button.get() ) ) & im_result::left_clicked )
		{
			log_msg( fmt::format( "CLICKED : {}", iter.name ) );
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
