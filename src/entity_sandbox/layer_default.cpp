
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	test_entity = add_entity<w_entity>();
	test_entity->set_transform( { v_window_hw, v_window_hh }, 0, 1 );
	test_entity->add_component<ec_sprite>()->init( "sub_cartoon_star" );
	test_entity->add_component<ec_emitter>()->init( "background_fire_up" );
}

void layer_default::draw()
{
	w_layer::draw();
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			test_entity->set_position( engine->input->mouse_vwindow_pos );
		}
	}

	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::key_left_bracket )
		{
			test_entity->set_scale( test_entity->scale - 0.25f );
		}

		if( evt->input_id == input_id::key_right_bracket )
		{
			test_entity->set_scale( test_entity->scale + 0.25f );
		}

		if( evt->input_id == input_id::key_c )
		{
			for( auto& e : entities )
			{
				if( e.get() != test_entity )
				{
					e->set_life_cycle( life_cycle::dying );
				}
			}
		}

		if( evt->input_id == input_id::key_space )
		{
			auto e = add_entity<w_entity>();
			e->set_transform( engine->input->mouse_vwindow_pos, 0, 1 );
			//e->add_component<ec_sprite>()->init( "sub_cartoon_star" );
			e->add_component<ec_emitter>()->init( "background_fire_up" );
		}
	}

	return true;
}
