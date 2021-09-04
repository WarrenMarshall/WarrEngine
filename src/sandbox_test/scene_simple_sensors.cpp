
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

bool E_Sensor_Player::on_touching_begin( Simple_Collision_Body* sensor )
{
	log( "TOUCHED_BEGIN : {}", g_engine->clock.now() );
	return true;
};

bool E_Sensor_Player::on_touching( Simple_Collision_Body* sensor )
{
	//log( "TOUCHING : {}", g_engine->clock.now() );
	return true;
};

bool E_Sensor_Player::on_touching_end( Simple_Collision_Body* sensor )
{
	log( "TOUCHED_END : {}", g_engine->clock.now() );
	return true;
};

// ----------------------------------------------------------------------------

Scene_Simple_Sensors::Scene_Simple_Sensors()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void Scene_Simple_Sensors::pushed()
{
	Scene::pushed();

	g_engine->render.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// KINEMATIC CIRCLE
	{
		auto e = add_entity<E_Sensor_Player>();
		e->tag = H( "the_player" );
		e->simple.type = e_sc_type::kinematic;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_circle( 32.f );
			ec->set_collision_flags( coll_flags.player, coll_flags.world | coll_flags.sensor );
		}
	}

	// SENSORS

	{
		auto e = add_entity<Entity>( "world" );
		e->simple.type = e_sc_type::stationary;

		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw / 4.f, 0.f } );
			ec->set_as_circle( 16.f );
			ec->set_body_collider_type( e_sc_body_collider_type::sensor );
			ec->set_collision_flags( coll_flags.sensor, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw / 2.f, 16.f } );
			ec->set_as_circle( 32.f );
			ec->set_body_collider_type( e_sc_body_collider_type::sensor );
			ec->set_collision_flags( coll_flags.sensor, 0 );
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

void Scene_Simple_Sensors::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Simple_Sensors::draw_ui()
{
	Scene::draw_ui();
}

void Scene_Simple_Sensors::update()
{
	Scene::update();
}

bool Scene_Simple_Sensors::on_input_pressed( const Input_Event* evt )
{
	return false;
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
