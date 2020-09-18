
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	world_geo = add_entity<w_entity>();
	world_geo->add_component<ec_b2d_static>()->init_as_box( { 100, 150 }, 128, 3 );
	world_geo->add_component<ec_b2d_static>()->init_as_circle( { 90, 100 }, 12 );
	world_geo->add_component<ec_b2d_static>()->init_as_box( { 114, 65}, 12, 12 );
	world_geo->add_component<ec_b2d_static>()->init_as_circle( { 50, 120 }, 12 );
	world_geo->add_component<ec_b2d_static>()->init_as_circle( { 150, 150 }, 32 );

	player = add_entity<w_entity>();
	player->set_transform( { 100, 0 }, 0, 1 );
	player->add_component<ec_sprite>()->init( "sprite_mario" );
	//player->add_component<ec_b2d_dynamic>()->init_as_box( 8, 10 );
	player->add_component<ec_b2d_dynamic>()->init_as_circle( 10 );
}

void layer_default::update()
{
	w_layer::update();
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
			//player->
		}
	}

	return true;
}
