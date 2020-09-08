
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	engine->window->set_mouse_mode( mouse_mode::hidden );

	player = add_entity<e_player>( { 32, 64 }, 0, 1 );
	player->add_component<ec_sprite>()->init( "subtex_player" );
	player->add_component<ec_collider>()->init_as_box( { -8,-8,16,16 } );
	//player->add_component<ec_collider>()->init_as_circle( 12 );
	player_force_right = player->add_component<ec_force_dir_accum>()->init( 90, 300, 100 );
	player_force_left = player->add_component<ec_force_dir_accum>()->init( 270, 300, 100 );
	//efc_player_input = player->add_component<ec_force_constant>()->init( 0, 0 );
	player->debug_draw_collision = true;

	player->collision_layer = cl_player;
	player->collides_with = cl_world;

	world_geo = add_entity<w_entity>();
	auto ec = world_geo->add_component<ec_collider>()->init_as_box( { 0, v_window_h - 32, v_window_w, 32 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 32, 150, 128, 16 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 150, 200, 64, 40 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 240, 190, 128, 16 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 240, 70, 16, 64 } );

	world_geo->debug_draw_collision = true;

	world_geo->collision_layer = cl_world;
}

constexpr float gravity_per_sec = 9.81f;

void layer_default::update_physics()
{
	w_layer::update_physics();

	//static float player_speed = 50.0f;

	w_vec2 delta = engine->input->axis_value_of( input_id::controller_left_stick );

	player_force_right->inactive();
	player_force_left->inactive();

	if( delta.x != 0.0f || delta.y != 0.0f )
	{
		if( delta.x < 0.0f )
		{
			player_force_left->active();
		}
		else
		{
			player_force_right->active();
		}
		//player->set_pos(
		//	{
		//		player->pos.x + ( ( delta.x * player_speed ) * engine->time->FTS_step_value_s ),
		//		player->pos.y + ( ( delta.y * player_speed ) * engine->time->FTS_step_value_s )
		//	}
		//);
	}
}

void layer_default::update()
{
	w_layer::update();
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
