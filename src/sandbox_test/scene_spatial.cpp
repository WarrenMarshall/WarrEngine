
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
	Rect rc( -viewport_w, -viewport_h, viewport_w * 2.f, viewport_h * 2.f );
	e->set_pos( rc.find_random_pos_within() );
	e->set_scale( Random::getf_range( 1.f, 2.f ) );
	e->set_pickable();
	e->rs_opt.color = make_color( e_pal::brighter );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
		ec->rs_opt.color = make_color( e_pal::brightest );
	}
	{
		auto ec = e->add_component<Simple_Collision_Body>();
		if( Random::getb() )
		{
			ec->set_as_circle( 8.f );
		}
		else
		{
			ec->set_as_centered_box( 16.f, 16.f );
		}
		ec->set_collision_flags( coll_flags.skull, coll_flags.skull );
	}
}

void Scene_Spatial::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::custom );

	Rect rc( -viewport_w, -viewport_h, viewport_w * 2.f, viewport_h * 2.f );
	spatial_map.init( rc, 4 );

	// kinematic circle
	{
		auto e = add_entity<Entity>();
		e->tag = H( "main_ball" );
		{
			{
				auto ec = e->add_component<Simple_Collision_Body>();

				ec->set_as_polygon( Geo_Util::generate_convex_shape( 6, 16.f ) );
				//ec->set_as_circle( 16.f );
				//ec->set_as_centered_box( 16.f, 16.f );
				//ec->set_as_box( 16.f, 16.f );
				ec->set_collision_flags( coll_flags.player, coll_flags.skull );
			}
			{
				auto ec = e->add_component<Primitive_Shape_Component>();
				ec->add_shape( e_primitive_shape::point );
			}
		}

		player_shape = e;
	}

	for( auto x = 0 ; x < 64 ; ++x )
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

	// show entities that COULD collide with player shape

	auto potential_entities = spatial_map.get_potential_entity_colliding_set( player_shape );

	Render::state->color = make_color( Color::teal, 0.5f );
	for( auto& e : potential_entities )
	{
		Render::draw_line( player_shape->get_transform()->pos, e->get_transform()->pos );
	}
}

void Scene_Spatial::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Spatial Partitioning" );
}

bool Scene_Spatial::on_input_pressed( const Input_Event* evt )
{
	// delete entities with right click
	if( evt->input_id == e_input_id::mouse_button_right )
	{
		auto pick_id = Render::sample_pick_id_at( Coord_System::window_to_viewport_pos( evt->mouse_pos ) );
		auto e = find_entity_by_pick_id( pick_id );

		if( e && e != player_shape )
		{
			e->set_life_cycle( e_life_cycle::dying );
		}
	}

	return false;
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

					player_shape->set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}
