
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	test_entity = add_entity<w_entity>( { v_window_hw, v_window_hh }, 0, 1 );
	test_entity->add_component<ec_collider>()->init_as_circle( 6 );
	test_entity->debug_draw_collision = true;
}

void layer_default::draw()
{
	w_layer::draw();
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::key_space )
		{
			auto efx = add_entity<w_entity_fx>( test_entity->pos, 0, 1 );
			efx->add_component<ec_emitter>()->init( "powup_pickup" );
			efx->add_component<ec_sound>()->init( "powup_hit" );
		}
	}

	return true;
}
