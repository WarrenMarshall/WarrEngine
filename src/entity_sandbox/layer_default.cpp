
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	test_entity = add_entity<w_entity>();
	test_entity->set_transform( { v_window_hw, v_window_hh }, 0, 1 );
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
			//auto efx = add_entity<w_entity_fx>();
			//efx->set_transform( test_entity->pos, 0, 1 );
			//efx->add_component<ec_emitter>()->init( "powup_pickup" );
			//efx->add_component<ec_sound>()->init( "powup_hit" );
		}
	}

	return true;
}
