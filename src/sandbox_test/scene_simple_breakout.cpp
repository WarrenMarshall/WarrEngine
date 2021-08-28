
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static Bit_Flag_Generator collision_bits = 1;

static const unsigned scene_simple_breakout_coll_ball = collision_bits.get();
static const unsigned scene_simple_breakout_coll_paddle = collision_bits.get();
static const unsigned scene_simple_breakout_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

bool e_ball::on_collided( simple_collision::Pending_Collision& coll )
{
	if( coll.entity_b->tag == H( "THE_PADDLE" ) )
	{
	}

	return false;
}

// ----------------------------------------------------------------------------

bool e_paddle::on_collided( simple_collision::Pending_Collision& coll )
{
	if( coll.entity_b->tag == H( "BALL" ) )
	{
		auto dir_from_center = coll.entity_b->get_transform()->pos - get_transform()->pos;
		coll.entity_b->set_force( { dir_from_center, coll.entity_b->velocity.get_size() } );

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

scene_simple_breakout::scene_simple_breakout()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_breakout::spawn_ball()
{
	auto e = add_entity<e_ball>();
	e->tag = H( "BALL" );
	e->set_pos( { 0.f, -64.f } );
	e->simple.friction = 0.0;
	e->simple.is_bouncy = true;
	{
		auto ec = e->add_component<Primitve_Shape_Component>();
		ec->add_shape( primitive_shape::point );
	}
	{
		auto ec = e->add_component<Simple_Collision_Body>();
		ec->set_as_circle( 12.f );
		ec->set_collision_flags( scene_simple_breakout_coll_ball, scene_simple_breakout_coll_geo | scene_simple_breakout_coll_paddle );
	}

	e->add_impulse( { Random::get_random_unit_vector(), 2.5f } );
}

void scene_simple_breakout::pushed()
{
	scene::pushed();

	g_engine->renderer.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// paddle
	{
		auto e = add_entity<e_paddle>();
		e->tag = H( "THE_PADDLE" );
		e->set_pos( { 0.f, 0.f } );
		e->simple.friction = 0.5f;
		e->simple.type = sc_type::kinematic;
		auto paddle_w = 200.f;
		auto paddle_h = 16.f;
		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( primitive_shape::point );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_centered_box( paddle_w, paddle_h );
			ec->set_collision_flags( scene_simple_breakout_coll_paddle, scene_simple_breakout_coll_ball );
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
		e->simple.type = sc_type::stationary;

		for( int i = 0 ; i < num_primitives ; ++i )
		{
			auto x = Random::getf_range( -viewport_hw, viewport_hw );
			auto y = Random::getf_range( -viewport_hw, viewport_hw );
			auto w = Random::getf_range( 16.f, 80.f );
			auto h = Random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->set_as_centered_box( w, h );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
			}
		}

		for( int i = 0 ; i < num_primitives ; ++i )
		{
			auto x = Random::getf_range( -viewport_hw, viewport_hw );
			auto y = Random::getf_range( -viewport_hw, viewport_hw );
			auto r = Random::getf_range( 8.f, 40.f );

			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
			}
		}

		// 4 walls
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}

		world_geo = e;
	}
}

void scene_simple_breakout::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	//render::draw_world_axis();
}

void scene_simple_breakout::draw_ui()
{
	scene::draw_ui();
}

void scene_simple_breakout::update()
{
	scene::update();
}

bool scene_simple_breakout::on_input_pressed( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_button_y:
		case input_id::key_space:
		{
			spawn_ball();
		}
		break;
	}

	return false;
}

bool scene_simple_breakout::on_input_held( const Input_Event* evt )
{

	return false;
}

bool scene_simple_breakout::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			paddle->add_force( { evt->delta * Vec2::x_axis, 150.f } );

			return true;
		}
		break;
	}

	return false;
}
