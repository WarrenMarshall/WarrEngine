
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	engine->window->set_mouse_mode( mouse_mode::hidden );

	background_gradient = engine->get_asset<a_gradient>( "background_gradient" );

	player = add_entity<e_player>( { 32, 64 }, 0, 1 );
	player->debug_draw_collision = true;
	player->collision_layer = cl_player;
	player->collides_with = cl_world;
	player->add_component<ec_sprite>()->init( "subtex_player" );
	player->add_component<ec_collider>()->init_as_box( { -8,-8,16,16 } );
	player_force_right = player->add_component<ec_force_dir_accum>()->init( 90, 300, 100 );
	player_force_left = player->add_component<ec_force_dir_accum>()->init( 270, 300, 100 );

	// ground plane
	auto ent = add_entity<w_entity>();
	ent->debug_draw_collision = true;
	ent->collision_layer = cl_world;
	ent->add_component<ec_collider>()->init_as_box( { 0, v_window_h - 32, v_window_w, 32 } );

	// test wall a
	ent = add_entity<w_entity>();
	ent->debug_draw_collision = true;
	ent->collision_layer = cl_world;
	colliders.push_back( (ec_collider*) ( ent->add_component<ec_collider>()->init_as_box( { 200, 50, 16, 64 } ) ) );
}

void layer_default::update_physics()
{
	w_layer::update_physics();

	w_vec2 delta = engine->input->axis_value_of( input_id::controller_left_stick );

	player_force_right->decay();
	player_force_left->decay();

	if( delta.x != 0.0f )
	{
		if( delta.x < 0.0f )
		{
			player_force_left->add_impulse();
		}
		else
		{
			player_force_right->add_impulse();
		}
	}
}

void layer_default::update()
{
	w_layer::update();
}

void layer_default::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->draw( background_gradient, { 0, 0, v_window_w, v_window_h } )
		->end();
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	//if( evt->event_id == event_id::input_released )
	//{
	//	if( evt->input_id == input_id::controller_button_a )
	//	{
	//		if( is_entity_on_ground( this, player ) )
	//		{
	//			player->add_component<ec_force_decay>()->init( 0, 250, 500 );
	//		}
	//	}
	//}

	return true;
}
