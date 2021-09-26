
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
	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	e->set_pos( rc.find_random_pos_within() );
	e->rs_opt.color = make_color( e_pal::brighter );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
		ec->rs_opt.color = make_color( e_pal::brightest );
	}
	{
		auto ec = e->add_component<Simple_Collision_Body>();
		//ec->set_as_circle( 16.f );
		ec->set_as_centered_box( 16.f, 16.f );
		ec->set_collision_flags( coll_flags.skull, coll_flags.skull );
	}
}

void Scene_Spatial::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::custom );

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	spatial_map.init( rc, 2 );

	// kinematic circle
	{
		auto e = add_entity<Entity>();
		e->tag = H( "main_ball" );
		{
			{
				{
					auto ec = e->add_component<Simple_Collision_Body>();

					ec->set_as_polygon( Geo_Util::generate_convex_shape( 6, 24.f ) );
					//ec->set_as_circle( 24.f );
					//ec->set_as_centered_box( 24.f, 24.f );
					//ec->set_as_box( 24.f, 24.f );
					ec->set_collision_flags( coll_flags.player, coll_flags.skull );
				}
			}
			{
				auto ec = e->add_component<Primitive_Shape_Component>();
				ec->add_shape( e_primitive_shape::point );
			}
		}
	}

	for( auto x = 0 ; x < 116 ; ++x )
	{
		spawn_entity();
	}
}

void Scene_Spatial::draw()
{
	//draw_tiled_background();
	Scene::draw();
	//Render::draw_world_axis();

	spatial_map.debug_draw();
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
			if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
			{
				if( !evt->shift_down and !evt->control_down )
				{
					auto world_pos = Coord_System::window_to_world_pos( evt->mouse_pos );

					auto e = find_entity( H( "main_ball" ) );
					e->set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}
