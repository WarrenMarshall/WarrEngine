
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Spatial::Scene_Spatial()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Spatial::spawn_entity()
{
	auto e = add_entity<Entity>();
	e->set_pos( qt.head.bounds.find_random_pos_within() );
	e->rs_opt.color = make_color( e_pal::brighter );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
		ec->rs_opt.color = make_color( e_pal::middle );
	}
}

void Scene_Spatial::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::custom );

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	qt.set_bounds( rc );

	spawn_entity();
	spawn_entity();
	spawn_entity();
	spawn_entity();
	spawn_entity();
}

void Scene_Spatial::draw()
{
	//draw_tiled_background();
	Scene::draw();
	//Render::draw_world_axis();

	qt.debug_draw();
}

void Scene_Spatial::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Spatial Partitioning" );
}

bool Scene_Spatial::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::mouse:
		{
		}
		break;

		case e_input_id::mouse_wheel:
		{
		}
		break;
	}

	return false;
}
