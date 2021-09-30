
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

bool E_Sensor_Player::on_touching_begin( Simple_Collision_Body* sensor )
{
	Entity::on_touching_begin( sensor );

	touch_count++;
	if( touch_count )
	{
		rs_opt.color = make_color( e_pal::brighter );
	}
	return true;
};

bool E_Sensor_Player::on_touching_end( Simple_Collision_Body* sensor )
{
	Entity::on_touching_end( sensor );

	touch_count--;
	if( !touch_count )
	{
		rs_opt.color = make_color( e_pal::darker );
	}
	return true;
};

// ----------------------------------------------------------------------------

Scene_Constraints::Scene_Constraints()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void Scene_Constraints::pushed()
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
			ec->add_shape( e_primitive_shape::circle, 16.f );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_circle( 32.f );
			ec->set_collision_flags( coll_flags.player, coll_flags.world | coll_flags.sensor );
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
	}
}

void Scene_Constraints::draw()
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

void Scene_Constraints::draw_ui()
{
	Scene::draw_ui();
}

void Scene_Constraints::update()
{
	Scene::update();
}

bool Scene_Constraints::on_input_pressed( const Input_Event* evt )
{
	return false;
}

bool Scene_Constraints::on_input_motion( const Input_Event* evt )
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