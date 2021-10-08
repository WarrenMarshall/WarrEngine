
#include "app_header.h"

using namespace war;

Scene_Simple_Sensors::Scene_Simple_Sensors()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void Scene_Simple_Sensors::spawn_sensor()
{
	auto e = add_entity<Entity>();
	e->simple.type = e_sc_type::stationary;
	e->set_pos( { -viewport_hw + ( Random::getf() * viewport_w ), -viewport_hh + ( Random::getf() * viewport_h ) } );
	{
		auto ec = e->add_component<Sprite_Component>();
		ec->init( "tex_skull" );
	}
	{
		auto ec = e->add_component<Simple_Collision_Body>();
		ec->set_sensor_as_continuous();
		ec->set_collision_flags( coll_flags.sensor, 0 );

		static int32_t last_spawned_type = 0;
		last_spawned_type++;
		switch( last_spawned_type % 3 )
		{
			case 0:
			{
				ec->set_as_centered_box( Random::getf_range( 10.0f, 30.0f ), Random::getf_range( 10.0f, 30.0f ) );
			}
			break;

			case 1:
			{
				ec->set_as_circle( Random::getf_range( 20.0f, 40.0f ) );
			}
			break;

			case 2:
			{
				auto s = Random::geti_range( 3, 8 );
				auto r = Random::getf_range( 10.f, 30.0f );
				ec->set_as_polygon( Geo_Util::generate_convex_shape( s, r ) );
			}
			break;
		}
	}
}

void Scene_Simple_Sensors::pushed()
{
	Scene::pushed();

	spatial_map.set_bounds( { -viewport_hw, -viewport_hh, viewport_w, viewport_h } );

	g_engine->render.debug.draw_colliders = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// KINEMATIC CIRCLE
	{
		auto e = add_entity<Entity>();
		e->tag = H( "the_player" );
		e->simple.type = e_sc_type::kinematic;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_circle, 32.f );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_circle( 32.f );
			ec->set_collision_flags( coll_flags.player, coll_flags.world | coll_flags.sensor );
		}
	}

	// SENSORS

	{
		for( auto x = 0 ; x < 10 ; ++x )
		{
			spawn_sensor();
		}
	}

	// WORLD GEO

	{
		auto e = add_entity<Entity>( "world" );
		e->simple.type = e_sc_type::stationary;

		// 4 walls
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}

		world_geo = e;
	}
}

bool Scene_Simple_Sensors::on_input_motion( const Input_Event* evt )
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

					auto e = find_entity( H( "the_player" ) );
					e->set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}

bool Scene_Simple_Sensors::on_entity_and_sensor_touching_begin( Entity* entity, Simple_Collision_Body* sensor )
{
	sensor->parent_entity->rs_opt.color = Color::red;
	return true;
}

bool Scene_Simple_Sensors::on_entity_and_sensor_touching_end( Entity* entity, Simple_Collision_Body* sensor )
{
	sensor->parent_entity->rs_opt.color = Color::white;
	return true;
}

