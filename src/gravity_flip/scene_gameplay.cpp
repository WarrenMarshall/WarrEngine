
#include "app_header.h"

using namespace war;

scene_gameplay::scene_gameplay()
{
	flags.is_debug_physics_scene = true;
	flags.requires_controller = true;
}

void scene_gameplay::draw_ui()
{
	scene::draw_ui();

	render::draw_string( std::format( "{:.1f}, {:.1f}",
		player->velocity.x, player->velocity.y ), vec2( 8.f, 8.f ) );
	render::draw_string( std::format( "In air : {}", player->in_air ), vec2( 8.f, 18.f ) );
	render::draw_string( std::format( "{:.1f}, {:.1f}",
		player->get_transform()->pos.x, player->get_transform()->pos.y ), vec2( 8.f, 28.f ) );
}

void scene_gameplay::draw()
{
	scene::draw();

	render::draw_world_axis();
}

f_decl_tile_map_spawn_entity( spawn_entity )
{
	auto gameplay_scene = (scene_gameplay*)scene;

	switch( tile->idx )
	{
		case 15:
		{
			auto tmc = gameplay_scene->world->get_component<ec_tile_map>();

			auto e = scene->add_entity<entity>();
			e->set_pos( vec2( tile->x_idx * tmc->tile_map->tile_sz, tile->y_idx * tmc->tile_map->tile_sz ) );
			e->add_delta_pos( vec2( tmc->tile_map->tile_sz / 2.f, tmc->tile_map->tile_sz / 2.f ) );
			e->affected_by_gravity = true;

			{
				auto ec = e->add_component<ec_sprite>();
				ec->init( "anim_player_run" );
			}
			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_collider_type( simple_collider_type::solid );
				//ec->set_as_centered_box( 16.f, 16.f );
				ec->set_as_circle( 8.0f );

				ec->set_collision_flags( coll_player, coll_world );
			}
		#if 1
			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_collider_type( simple_collider_type::sensor );
				ec->set_as_circle( 4.0f );
				ec->get_transform()->set_pos( { 0.f, 8.f } );

				ec->set_collision_flags( coll_player, coll_world );
			}
		#endif

			gameplay_scene->player = e;

			tile->idx = tile_map_asset::tile::empty;

		}
		break;
	}
}

void scene_gameplay::pushed()
{
	viewport_pivot = vec2::zero;

	g_engine->window.set_mouse_mode( mouse_mode::os );

	// world

	{
		world = add_entity<entity>();

		{
			auto ec = world->add_component<ec_tile_map>();
			ec->set_collision_flags( coll_world, 0 );
			ec->init( "ts_neon", "tm_level_01" );
			ec->spawn_entities( this, spawn_entity );
		}
	}
}

void scene_gameplay::update()
{

	scene::update();

}

bool scene_gameplay::on_input_motion( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_left_stick )
	{
		float force = 2.0f;
		vec2 delta = evt->delta;

		player->set_force_x( delta.x * force );
	}

	return false;
}

bool scene_gameplay::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a )
	{
		if( !player->in_air )
		{
			player->add_force( { 0.f, -4.0f } );
			return true;
		}
	}

	return false;
}
