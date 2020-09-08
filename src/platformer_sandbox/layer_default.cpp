
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	engine->window->set_mouse_mode( mouse_mode::hidden );

	player = add_entity<e_player>( { 200, 64 }, 0, 1 );
	player->add_component<ec_sprite>()->init( "subtex_player" );
	player->add_component<ec_collider>()->init_as_box( { -8,-8,16,16 } );
	//player->add_component<ec_collider>()->init_as_circle( 12 );
	//player->add_component<ec_force_constant>()->init( 180, 98.1f );
	//efc_player_input = static_cast<ec_force_constant*>( player->add_component<ec_force_constant>()->init( 0, 0 ) );
	player->debug_draw_collision = true;

	player->collision_layer = cl_player;
	player->collides_with = 0;

	world_geo = add_entity<w_entity>();
	auto ec = world_geo->add_component<ec_collider>()->init_as_box( { 0, v_window_h - 32, v_window_w, 32 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 32, 150, 128, 16 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 150, 200, 64, 40 } );
	world_geo->add_component<ec_collider>()->init_as_box( { 240, 190, 128, 16 } );
	world_geo->debug_draw_collision = true;

	world_geo->collision_layer = cl_world;
}

constexpr float gravity_per_sec = 9.81f;

bool is_entity_on_ground( w_layer* layer, w_entity* e )
{
	for( auto& ent : layer->entities )
	{
		if( ent.get() != e )
		{
			for( auto& ec : ent->components )
			{
				if( ec->type == component_type::collider )
				{
					ec_collider* collider = static_cast<ec_collider*>( ec.get() );

					c2Ray ray;
					ray.p = { e->pos.x, e->pos.y + 8 };	// trace from the bottom of the entities bounding box
					ray.d = { 0, 1 };					// down
					ray.t = 1;

					c2Raycast hit;

					if( c2CastRay( ray, &collider->get_collider(), NULL, collider->c2type, &hit ) )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void layer_default::update_physics()
{
	if( !is_entity_on_ground( this, player ) )
	{
		player->physics_cache.ending_pos.y += 98.0f * engine->time->FTS_step_value_s;
	}
}

void layer_default::update()
{
	w_layer::update();

	//efc_player_input->strength = 0.0f;
	//w_vec2 left_stick = engine->input->axis_value_of( input_id::controller_left_stick );
	//if( left_stick.x != 0.0f )
	//{
	//	if( left_stick.x < 0 )
	//	{
	//		efc_player_input->angle = 270;
	//		efc_player_input->strength = 350.0f * fabs( left_stick.x );
	//	}
	//	else
	//	{
	//		efc_player_input->angle = 90;
	//		efc_player_input->strength = 350.0f * fabs( left_stick.x );
	//	}
	//}
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::controller_button_a )
		{
			if( is_entity_on_ground( this, player ) )
			{
				player->add_component<ec_force_decaying>()->init( 0, 250, 500 );
			}
		}
	}

	return true;
}
