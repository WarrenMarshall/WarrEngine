
#include "app_header.h"

using namespace war;

scene_meshes::scene_meshes()
{
	flags.blocks_further_drawing = true;

	mesh_rotator = tween( 0, 360, 5000, tween_type::pingpong, tween_via::bounce );
	mesh_scaler = tween( 1.f, 2.f, 3000, tween_type::pingpong, tween_via::sinusoidal );
}

void scene_meshes::pushed()
{
	scene::pushed();

	// PLAYER

	{
		auto e = add_entity<entity>();
		e->set_pos( { -128.f, 0.f } );
		{
			mesh_player = e->add_component<ec_mesh>();
			mesh_player->init( "mesh_mario_jump" );
			mesh_player->get_transform()->set_scale( 1.25f );
		}
	}

	// CRATE

	{
		auto e = add_entity<entity>();
		e->set_pos( { 128.f, 0.f } );
		{
			mesh_crate = e->add_component<ec_mesh>();
			mesh_crate->init( "mesh_crate" );
		}
	}

	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_meshes::draw()
{
	mesh_player->get_transform()->set_angle( *mesh_rotator );
	mesh_crate->get_transform()->set_scale( *mesh_scaler );

	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_meshes::draw_ui()
{
	scene::draw_ui();
	draw_title( "Meshes" );
}
