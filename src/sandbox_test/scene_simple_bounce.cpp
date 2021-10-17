
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Scene_Simple_Bounce::Scene_Simple_Bounce()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

Entity* Scene_Simple_Bounce::spawn_shape()
{
	constexpr auto radius = 12.f;
	auto e = add_entity<Entity>();
	e->set_scale( Random::getf_range( 1.0f, 1.5f ) );
	e->simple.set_friction( 0.f );
	e->simple.is_bouncy = true;

	{
		auto ec = e->add_component<Simple_Collision_Body>();

		static int32_t last_spawned_type = 0;
		last_spawned_type++;

		//last_spawned_type = 1;
		switch( last_spawned_type % 3 )
		{
			case 0:
			{
				ec->set_as_centered_box( radius * Random::getf_range( 1.0f, 3.0f ), radius * Random::getf_range( 1.0f, 3.0f ) );
			}
			break;

			case 1:
			{
				ec->set_as_circle( radius * Random::getf_range( 0.5f, 1.5f ) );
			}
			break;

			case 2:
			{
				auto s = Random::geti_range( 3, 8 );
				auto r = radius * Random::getf_range( 0.5f, 2.0f );
				ec->set_as_polygon( Geo_Util::generate_convex_shape( s, r ) );
			}
			break;
		}

		ec->set_collision_flags( coll_flags.shape, coll_flags.geo | coll_flags.shape );
	}
	{
		auto ec = e->add_component<Primitive_Shape_Component>();
		ec->rs_opt.color = make_color( Color::white, 1.0f );
		ec->add_shape( e_primitive_shape::point );
	}

	e->add_impulse( { Random::get_random_unit_vector(), 2.f } );

	first_time = false;

	return e;
}

void Scene_Simple_Bounce::pushed()
{
	Scene::pushed();

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	spatial_map.set_bounds( rc );

	sc_world.settings.max_pos_iterations = 1;

#ifndef _RELEASE
	g_engine->render.debug.draw_colliders = true;
#endif
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// WORLD GEO

	{
		int num_colliders = 3;

		auto e = add_entity<Entity>();
		e->tag = H( "world_geo" );
		e->simple.type = e_sc_type::stationary;

		for( int i = 0 ; i < num_colliders ; ++i )
		{
			auto x = Random::getf_range( -viewport_hw, viewport_hw );
			auto y = Random::getf_range( -viewport_hw, viewport_hw );
			auto w = Random::getf_range( 16.f, 80.f );
			auto h = Random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<Simple_Collision_Body>();
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
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( coll_flags.geo, 0 );
			}
		}

		// 4 walls
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
		}
	}
}

void Scene_Simple_Bounce::draw()
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

void Scene_Simple_Bounce::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Bounce" );

}

void Scene_Simple_Bounce::post_update()
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

bool Scene_Simple_Bounce::on_input_pressed( const Input_Event* evt )
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

	return false;
}
