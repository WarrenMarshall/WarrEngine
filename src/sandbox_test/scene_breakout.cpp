
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

bool E_Breakout_Paddle::on_collided( collision::Pending_Collision& coll )
{
	if( coll.entity_b->tag == H( "BALL" ) )
	{
		auto dir_from_center = coll.entity_b->get_transform()->pos - get_transform()->pos;
		coll.entity_b->add_impulse( { dir_from_center, coll.entity_b->velocity.get_size() } );
		coll.entity_b->velocity = Vec2::zero;

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

Scene_Breakout::Scene_Breakout()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void Scene_Breakout::spawn_ball()
{
	auto e = add_entity<Entity>();
	e->tag = H( "BALL" );
	e->set_pos( { 0.f, -64.f } );
	e->simple.flags.is_bouncy = true;
	{
		auto ec = e->add_component<Primitive_Shape_Component>();
		ec->add_shape( e_primitive_shape::point );
	}
	{
		auto ec = e->add_component<Collision_Body_Component>();
		ec->set_as_circle( 12.f );
		ec->set_collision_flags( coll_flags.ball, coll_flags.geo | coll_flags.ball | coll_flags.paddle );
	}

	e->add_impulse( { Random::get_random_unit_vector(), 2.5f } );
}

void Scene_Breakout::pushed()
{
	Scene::pushed();

#ifndef _RELEASE
	g_engine->render.debug.draw_colliders = true;
#endif
	// paddle
	{
		auto e = add_entity<E_Breakout_Paddle>();
		e->tag = H( "THE_PADDLE" );
		e->set_pos( { 0.f, 0.f } );
		e->simple.type = e_physics_body_type::kinematic;
		auto paddle_w = 200.f;
		auto paddle_h = 16.f;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->set_as_centered_box( paddle_w, paddle_h );
			ec->set_collision_flags( coll_flags.paddle, coll_flags.ball );
		}

		paddle = e;
	}

	// BALL

	spawn_ball();

	// WORLD GEO

	{
		auto num_primitives = 2;
		auto e = add_entity<Entity>();
		e->tag = H( "world_geo" );
		e->simple.type = e_physics_body_type::stationary;

		for( int i = 0 ; i < num_primitives ; ++i )
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

		for( int i = 0 ; i < num_primitives ; ++i )
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

		world_geo = e;
	}
}

void Scene_Breakout::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	Scene::draw();
	//render::draw_world_axis();
}

void Scene_Breakout::draw_ui()
{
	Scene::draw_ui();
}

void Scene_Breakout::update()
{
	Scene::update();
}

bool Scene_Breakout::on_input_pressed( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::gamepad_button_y:
		case e_input_id::key_space:
		{
			spawn_ball();
		}
		break;
	}

	return false;
}

bool Scene_Breakout::on_input_held( const Input_Event* evt )
{

	return false;
}

bool Scene_Breakout::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			paddle->add_force( { evt->delta * Vec2::x_axis, 150.f } );

			return true;
		}
		break;
	}

	return false;
}
