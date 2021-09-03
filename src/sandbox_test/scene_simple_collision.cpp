
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static Bit_Flag_Generator collision_bits = 1;

static const uint16_t scene_simple_coll_ball = collision_bits.get();
static const uint16_t scene_simple_coll_world = collision_bits.next();
static const uint16_t scene_simple_coll_dynamic_object = collision_bits.get();

Scene_Simple_Collision::Scene_Simple_Collision()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void Scene_Simple_Collision::pushed()
{
	Scene::pushed();

	g_engine->render.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// KINEMATIC CIRCLE
	{
		auto e = add_entity<Entity>();
		e->tag = H( "main_ball" );
		e->simple.type = e_sc_type::kinematic;
		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_circle( 32.f );
			ec->set_collision_flags( scene_simple_coll_ball, scene_simple_coll_dynamic_object );
		}
	}

	// WORLD GEO

	{
		auto e = add_entity<Entity>( "world" );
		e->simple.type = e_sc_type::stationary;

		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_h } );
			ec->set_as_box( 16.f, viewport_h * 2.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_h } );
			ec->set_as_box( 16.f, viewport_h * 2.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
		}

		world_geo = e;
	}
}

void Scene_Simple_Collision::draw()
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

void Scene_Simple_Collision::draw_ui()
{
	Scene::draw_ui();
}

void Scene_Simple_Collision::update()
{
	Scene::update();
}

void Scene_Simple_Collision::spawn_ball_at( Vec2 world_pos )
{
	auto e = add_entity<Entity>( "ball" );
	e->set_pos( world_pos );
	e->rs_opt.color = Color( Random::getf(), Random::getf(), Random::getf() );
	e->make_pickable();
	e->simple.friction = 0.01f;
	e->simple.is_affected_by_gravity = true;
	e->simple.max_velocity_y = { -5.f, sc_world->settings.gravity_max_speed };
	e->simple.is_bouncy = true;
	{
		float random_radius = Random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_circle( random_radius );
			ec->set_collision_flags( scene_simple_coll_dynamic_object, scene_simple_coll_world | scene_simple_coll_ball );
		}
	}
}

void Scene_Simple_Collision::spawn_box_at( Vec2 world_pos )
{
	float base_size = 8.f;
	float w = Random::getf_range( base_size, base_size * 10.f );
	float h = ( base_size * 15.f ) - w;
	Rect rc_box = { 0.f, 0.f, w, h };

	auto e = add_entity<Entity>();
	e->set_pos( world_pos );
	e->make_pickable();
	e->simple.friction = 0.5f;
	e->simple.is_affected_by_gravity = true;
	e->simple.is_bouncy = true;
	e->simple.max_velocity_y = { -5.f, sc_world->settings.gravity_max_speed };
	{
		float random_radius = Random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( e_primitive_shape::point );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_box( rc_box.w, rc_box.h );
			ec->set_collision_flags( scene_simple_coll_dynamic_object, scene_simple_coll_world | scene_simple_coll_ball );
		}
	}
}

bool Scene_Simple_Collision::on_input_pressed( const Input_Event* evt )
{
	// spawn ball at random location
	if( evt->input_id == e_input_id::key_r )
	{
		auto num_new_balls = 1;

		if( evt->shift_down )
		{
			num_new_balls = 20;
		}

		Bounding_Box spawn_area( { -viewport_hw + 8, -8.f }, { viewport_hw - 8, -viewport_hh } );

		for( auto x = 0 ; x < num_new_balls ; ++x )
		{
			spawn_ball_at( spawn_area.get_random_spot() );
		}
	}

	// delete entities with right click
	if( evt->input_id == e_input_id::mouse_button_right )
	{
		auto pick_id = Render::sample_pick_id_at( Coord_System::window_to_viewport_pos( evt->mouse_pos ) );
		auto e = find_entity_by_pick_id( pick_id );

		if( e )
		{
			e->set_life_cycle( e_life_cycle::dying );
			g_engine->find_asset<Sound_Asset>( "sfx_entity_delete" )->play();
		}
	}

	// shift_lclick to spawn new ball at mouse position
	if( evt->input_id == e_input_id::mouse_button_left and evt->shift_down )
	{
		auto world_click_location = Coord_System::window_to_world_pos( evt->mouse_pos );
		spawn_ball_at( world_click_location );
		return true;
	}

	// control_lclick to spawn new box at mouse position
	if( evt->input_id == e_input_id::mouse_button_left and evt->control_down )
	{
		auto world_click_location = Coord_System::window_to_world_pos( evt->mouse_pos );
		spawn_box_at( world_click_location );
		return true;
	}

	return false;
}

bool Scene_Simple_Collision::on_input_motion( const Input_Event* evt )
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
