
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

			{
				auto ec = e->add_component<sprite_component>();
				ec->init( "anim_player_run" );
			}
			{
				auto ec = e->add_component<simple_collision_component>();
				//ec->set_as_centered_box( 24.f, 24.f );
				ec->set_as_circle( 8.f );
				ec->set_collision_flags( coll_player, coll_world );
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
	viewport_pivot = { viewport_hw, viewport_hh };

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
		player->add_linear_force( evt->delta * fixed_time_step::per_second( 150.f ) );
	}

	return false;
}

bool scene_gameplay::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a )
	{
		//player->fire();
		return true;
	}

	return false;
}
