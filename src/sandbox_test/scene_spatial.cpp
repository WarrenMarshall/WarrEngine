
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Spatial::Scene_Spatial()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Spatial::spawn_entity( Vec2 pos )
{
	auto e = add_entity<Entity>();
	e->flags.include_in_quad_tree = true;
	e->set_pos( pos );
	e->set_pickable();
	e->rs_opt.color = make_color( e_pal::lighter );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
		ec->rs_opt.color = make_color( e_pal::lightest );
	}
	{
		auto ec = e->add_component<Collision_Body_Component>();
		if( Random::getb() )
		{
			ec->set_as_circle( 8.f );
		}
		else
		{
			ec->set_as_centered_box( 16.f, 16.f );
		}
		ec->get_transform()->set_scale( Random::getf_range( 1.f, 2.f ) );
		ec->set_collision_flags( coll_flags.skull, coll_flags.skull );
		if( Random::getb() )
		{
			ec->set_sensor_as_continuous();
		}
	}
}

void Scene_Spatial::pushed()
{
	Scene::pushed();

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	spatial_map.set_bounds( rc );
	spatial_map.min_node_area = 32;
	spatial_map.set_max_nodes_in_pool( 350 );

	//get_transform()->set_scale( 0.5f );

	// PLAYER SHAPE
	{
		auto e = add_entity<Entity>();
		e->flags.include_in_quad_tree = true;
		e->tag = H( "main_ball" );
		e->flags.include_in_quad_tree = true;

		{
			auto ec = e->add_component<Collision_Body_Component>();

			//ec->set_as_circle( Random::getf_+range( 8.f, 16.f ) );
			//ec->set_as_polygon( Geo_Util::generate_convex_shape( Random::geti_range( 3, 7 ), Random::getf_range( 12.f, 24.f ) ) );
			ec->set_as_centered_box( 16.f, 16.f );
			ec->set_collision_flags( coll_flags.player, coll_flags.skull );
		}
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( "tex_player" );
		}

		player_shape = e;
	}

	for( auto x = 0 ; x < 50 ; ++x )
	{
		spawn_entity( rc.find_random_pos_within() );
	}
}

void Scene_Spatial::draw()
{
	draw_tiled_background();
	Scene::draw();
	Render::draw_world_axis();

	// show entities that COULD collide with player shape
	auto potential_entities = spatial_map.find_potentially_colliding_entities( player_shape );

	Render::state->color = make_color( Color::teal, 0.5f );
	for( auto& e : potential_entities )
	{
		Render::draw_line( player_shape->get_transform()->pos, e->get_transform()->pos );
	}

	// show ray trace indicator

	if( !trace_end.is_zero() )
	{
		Render::state->color = make_color( Color::yellow, 0.5f );
		Render::draw_line( player_shape->get_transform()->pos, trace_end );
	}
}

void Scene_Spatial::draw_ui()
{
	draw_title( "Spatial Partitioning" );
	Scene::draw_ui();
}

bool Scene_Spatial::on_input( const Input_Event* evt )
{
	if( Scene::on_input( evt ) )
	{
		return true;
	}

	if( evt->is_pressed() )
	{
		// delete entities with right click
		if( evt->input_id == e_input_id::mouse_button_right )
		{
			if( g_engine->input_mgr.is_shift_down() )
			{
				auto pick_id = Render::sample_pick_id_at( Coord_System::window_to_viewport_pos( evt->mouse_pos ) );
				auto e = find_entity_by_pick_id( pick_id );

				if( e && e != player_shape )
				{
					e->set_life_cycle( e_life_cycle::dying );
				}
			}
		}
		else if( evt->input_id == e_input_id::key_backspace )
		{
			trace_end = Vec2::zero;
		}
	}
	else if( evt->is_motion() )
	{
		switch( evt->input_id )
		{
			case e_input_id::mouse:
			{
				if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_left ) )
				{
					if( !g_engine->input_mgr.is_shift_down() and !g_engine->input_mgr.is_control_down() )
					{
						auto world_pos = Coord_System::window_to_world_pos( evt->mouse_pos );

						player_shape->set_pos( world_pos );

						return true;
					}
				}

				if( g_engine->input_mgr.is_button_held( e_input_id::mouse_button_right ) )
				{
					if( !g_engine->input_mgr.is_shift_down() and !g_engine->input_mgr.is_control_down() )
					{
						trace_end = Coord_System::window_to_world_pos( evt->mouse_pos );
						return true;
					}
				}
			}
			break;
		}
	}

	return false;
}
