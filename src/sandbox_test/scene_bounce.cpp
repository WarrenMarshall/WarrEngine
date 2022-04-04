
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Bounce::Scene_Bounce()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

Entity* Scene_Bounce::spawn_shape()
{
	constexpr auto radius = 12.f;
	auto e = add_entity<Entity>();
	e->set_scale( Random::getf_range( 1.0f, 1.5f ) );
	e->flags.include_in_quad_tree = true;
	e->collision.set_restitution( 0.0f );
	e->collision.set_friction( 0.5f );

	{
		auto ec = e->add_component<Collision_Body_Component>();

		static i32 last_spawned_type = 0;
		last_spawned_type++;

		switch( last_spawned_type % 3 )
		{
			case 0:	ec->set_as_centered_box( radius * Random::getf_range( 1.0f, 3.0f ), radius * Random::getf_range( 1.0f, 3.0f ) );	break;
			case 1: ec->set_as_circle( radius * Random::getf_range( 0.5f, 1.5f ) );	break;
			case 2:	ec->set_as_polygon( Geo_Util::generate_convex_shape( Random::geti_range( 3, 8 ), radius * Random::getf_range( 0.5f, 2.0f ) ) );	break;
		}

		ec->set_collision_flags( coll_flags.shape, coll_flags.geo | coll_flags.shape | coll_flags.ball );
	}
	{
		auto ec = e->add_component<Primitive_Shape_Component>();
		ec->rs_opt.color = make_color( Color::white, 1.0f );
		ec->add_shape( e_primitive_shape::point );
	}

	e->add_impulse( { Random::get_random_unit_vector(), 2.f } );

	return e;
}

void Scene_Bounce::pushed()
{
	Scene::pushed();

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	spatial_map.set_bounds( rc );

	sc_world.settings.max_pos_iterations = 1;

#ifndef _RELEASE
	g_engine->render.debug.draw_colliders = true;
#endif

	// BALL

	{
		auto e = add_entity<Entity>();
		e->flags.include_in_quad_tree = true;
		e->tag = H( "main_ball" );
		e->collision.type = e_physics_body_type::kinematic;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->set_as_circle( 24.f );
			ec->set_collision_flags( coll_flags.ball, coll_flags.shape | coll_flags.geo );
		}
	}

	// WORLD GEO

	{
		int num_colliders = 3;

		auto e = add_entity<Entity>();
		e->flags.include_in_quad_tree = true;
		e->tag = H( "world_geo" );
		e->collision.type = e_physics_body_type::stationary;

		for( int i = 0 ; i < num_colliders ; ++i )
		{
			auto x = Random::getf_range( -viewport_hw, viewport_hw );
			auto y = Random::getf_range( -viewport_hw, viewport_hw );
			auto w = Random::getf_range( 16.f, 80.f );
			auto h = Random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<Collision_Body_Component>();
				ec->set_as_centered_box( w, h );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( coll_flags.geo, 0 );
			}
		}

		for( int i = 0 ; i < num_colliders ; ++i )
		{
			auto x = Random::getf_range( -viewport_hw, viewport_hw );
			auto y = Random::getf_range( -viewport_hw, viewport_hw );
			auto r = Random::getf_range( 8.f, 40.f );

			{
				auto ec = e->add_component<Collision_Body_Component>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( coll_flags.geo, 0 );
			}
		}

		// 4 walls
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
	}
}

void Scene_Bounce::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
		Render::flush();
	}

	Scene::draw();
}

void Scene_Bounce::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Bounce" );
}

void Scene_Bounce::post_update()
{
	Scene::post_update();

	for( auto& e : entities )
	{
		auto pos = e->get_pos();

		if( pos.x < -viewport_hw )		pos.x = viewport_hw;
		if( pos.x > viewport_hw )		pos.x = -viewport_hw;
		if( pos.y < -viewport_hh )		pos.y = viewport_hh;
		if( pos.y > viewport_hh )		pos.y = -viewport_hh;

		e->set_pos( pos );
	}
}

bool Scene_Bounce::on_input( const Input_Event* evt )
{
	if( Scene::on_input( evt ) )
	{
		return true;
	}

	if( evt->is_pressed() )
	{
		switch( evt->input_id )
		{
			case e_input_id::gamepad_button_y:
			case e_input_id::key_space:
			{
				spawn_shape();
			}
			break;
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

						auto e = find_entity( H( "main_ball" ) );
						e->set_pos( world_pos );

						return true;
					}
				}
			}
			break;
		}
	}

	return false;
}
