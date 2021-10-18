
#include "app_header.h"

using namespace war;

Scene_Meshes::Scene_Meshes()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;

	mesh_rotator = Tween( 0, 360, 5000, e_tween_type::pingpong, e_tween_via::bounce );
	mesh_scaler = Tween( 1.f, 2.f, 3000, e_tween_type::pingpong, e_tween_via::sinusoidal );
}

void Scene_Meshes::pushed()
{
	Scene::pushed();

	// PLAYER

	{
		auto e = add_entity<Entity>();
		e->set_pos( { -128.f, 0.f } );
		{
			mesh_player = e->add_component<Mesh_Component>();
			mesh_player->init( "mesh_mario_jump" );
			mesh_player->get_transform()->set_scale( 1.25f );
		}
	}

	// CRATE

	{
		auto e = add_entity<Entity>();
		e->set_pos( { 128.f, 0.f } );
		{
			mesh_crate = e->add_component<Mesh_Component>();
			mesh_crate->init( "mesh_crate" );
		}
	}
}

void Scene_Meshes::draw()
{
	mesh_player->get_transform()->set_angle( *mesh_rotator );
	mesh_crate->get_transform()->set_scale( *mesh_scaler );

	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Meshes::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Meshes" );
}
