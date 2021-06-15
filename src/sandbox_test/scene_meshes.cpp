
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
	// mario
	{
		auto e = add_entity<entity>();
		e->transform_set_pos( { -128.f, 0.f } );
		{
			mesh_mario = e->add_component<mesh_component>();
			mesh_mario->init( "mesh_mario_jump" );
			mesh_mario->get_transform()->set_scale( 1.25f );
		}
	}

	// crate
	{
		auto e = add_entity<entity>();
		e->transform_set_pos( { 128.f, 0.f } );
		{
			mesh_crate = e->add_component<mesh_component>();
			mesh_crate->init( "mesh_crate" );
		}
	}

	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_meshes::draw()
{
	mesh_mario->get_transform()->set_angle( *mesh_rotator );
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
