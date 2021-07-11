
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
			auto tmc = gameplay_scene->world->get_component<tile_map_component>();

			auto e = scene->add_entity<entity>();
			e->set_pos( vec2( tile->x_idx * tmc->tile_map->tile_sz, tile->y_idx * tmc->tile_map->tile_sz ) );
			e->add_delta_pos( vec2( tmc->tile_map->tile_sz / 2.f, tmc->tile_map->tile_sz / 2.f ) );
			e->affected_by_gravity = true;

			{
				auto ec = e->add_component<sprite_component>();
				ec->init( "anim_player_run" );
			}
			{
				auto ec = e->add_component<simple_collision_body_component>();
				//ec->set_as_centered_box( 16.f, 16.f );
				ec->set_as_circle( 8.0f );
/*
				ec->set_as_polygon(
					{
						vec2( -8.f, -8.f ),
						vec2( +4.f, -8.f ),
						vec2( +12.f, 0.f ),
						vec2( +4.f, +8.f ),
						vec2( -8.f, +8.f ),
						vec2( -16.f, 0.f )
					}
				);
*/
				ec->set_collision_flags( coll_player, coll_world | coll_player );
				ec->rs_opt.color = make_color( color::orange );
			}

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
			auto ec = world->add_component<tile_map_component>();
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
		auto pos_a = player->get_transform()->pos;
		player->set_force_x( evt->delta.x * 2.0f );
		auto pos_b = player->get_transform()->pos;

		assert( pos_a == pos_b );
	}

	return false;
}

bool scene_gameplay::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a )
	{
		player->add_force( { 0.f, -4.0f } );
		return true;
	}

	return false;
}
