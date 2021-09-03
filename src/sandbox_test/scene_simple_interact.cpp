
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static Bit_Flag_Generator collision_bits = 1;

static const uint16_t scene_simple_coll_player = collision_bits.get();
static const uint16_t scene_simple_coll_geo = collision_bits.next();
static const uint16_t scene_simple_coll_npc = collision_bits.next();

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 350.f;
constexpr auto radius = 12.f;

// ----------------------------------------------------------------------------

Scene_Simple_Interact::Scene_Simple_Interact()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

Entity* Scene_Simple_Interact::spawn_player()
{
	auto e = add_entity<Entity>();
	e->debug_name = "player";
	e->set_pos( { 0.f, 0.f } );
	e->set_scale( 1.5f );
	e->simple.friction = 0.1f;
	{
		auto ec = e->add_component<Simple_Collision_Body>();

		switch( Random::geti_range( 0, 2 ) )
		{
			case 0:
			{
				ec->set_as_centered_box( radius * Random::getf_range( 0.5f, 3.0f ), radius * Random::getf_range( 0.5f, 3.0f ) );
			}
			break;

			case 1:
			{
				ec->set_as_circle( radius * Random::getf_range( 0.5f, 2.0f ) );
			}
			break;

			case 2:
			{
				auto s = Random::geti_range( 3, 8 );
				auto r = radius * Random::getf_range( 0.5f, 3.0f );
				ec->set_as_polygon( Geo_Util::generate_convex_shape( s, r ) );
			}
			break;
		}

		ec->set_collision_flags( scene_simple_coll_player, scene_simple_coll_geo | scene_simple_coll_player );
	}
	{
		auto ec = e->add_component<Primitve_Shape_Component>();
		ec->add_shape( e_primitive_shape::point );
	}

	first_player = false;

	return e;
}


void Scene_Simple_Interact::pushed()
{
	Scene::pushed();

	g_engine->render.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// PLAYER

	player = spawn_player();
	player->add_delta_pos( { 0.f, 80.f } );
	player->rs_opt.glow = 3.0f;	// player has a glow
	spawn_player();

	// HIT MARKER

	{
		auto e = add_entity<Entity>();
		e->tag = H( "hit_marker" );
		e->set_pos( { 0.f, 0.f } );
		e->rs_opt.z_bias = zdepth_debug_bias;
		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->rs_opt.color = make_color( Color::yellow );
		}

		hit_marker = e;

	}

	// ` GEO

	{
		int num_primitives = 4;

		auto e = add_entity<Entity>( "world" );
		e->simple.type = e_sc_type::stationary;

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
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
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
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
			}
		}

		// 4 walls
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_coll_geo, 0 );
		}

		world_geo = e;
	}
}

void Scene_Simple_Interact::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	Scene::draw();
	//render::draw_world_axis();

	if( b_show_ray )
	{
		Render::state->color = make_color( Color::green, 0.25f );
		auto start = player->get_pos();
		Render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void Scene_Simple_Interact::draw_ui()
{
	Scene::draw_ui();
	//draw_title( "Simple Collisions" );

	//render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
	//	player->velocity.x, player->velocity.y ), vec2( 8.f, 24.f ) );
}

void Scene_Simple_Interact::update()
{
	Scene::update();

	// show the raycast beam if the right stick is being pushed
	b_show_ray = g_engine->input_mgr.get_axis_state( e_input_id::gamepad_right_stick ).get_size_fast() > 0.f;

	follow_cam( player->get_transform() );
}

void Scene_Simple_Interact::reset_collision_trace_results()
{
	hit_marker->get_component<Primitve_Shape_Component>()->shapes.clear();

	for( auto& iter : world_geo->get_components<Simple_Collision_Body>() )
	{
		iter->rs_opt.color = make_color( Color::dark_teal );
	}
}

bool Scene_Simple_Interact::on_input_pressed( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::key_1:
		{
			auto ec = player->get_component<Simple_Collision_Body>();
			ec->set_as_centered_box( radius * Random::getf_range( 0.5f, 3.0f ), radius * Random::getf_range( 0.5f, 3.0f ) );
		}
		break;

		case e_input_id::key_2:
		{
			auto ec = player->get_component<Simple_Collision_Body>();
			ec->set_as_circle( radius * Random::getf_range( 0.5f, 2.0f ) );
		}
		break;

		case e_input_id::key_3:
		{
			auto ec = player->get_component<Simple_Collision_Body>();

			auto s = Random::geti_range( 3, 8 );
			auto r = radius * Random::getf_range( 0.5f, 3.0f );
			ec->set_as_polygon( Geo_Util::generate_convex_shape( s, r ) );
		}
		break;

		case e_input_id::gamepad_button_y:
		case e_input_id::key_space:
		{
			spawn_player();
		}
		break;

		case e_input_id::mouse_button_right:
		{
			auto pos = Coord_System::window_to_world_pos( evt->mouse_pos );
			player->set_pos( pos );
		}
		break;

		// ALL

		case e_input_id::gamepad_button_left_shoulder:
		case e_input_id::key_a:
		{
			reset_collision_trace_results();

			auto start = player->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::Raycast_All callback;
			sc_world->ray_cast( &callback, player, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<Primitve_Shape_Component>();
				ec->shapes.clear();

				for( auto& hit : callback.results )
				{
					ec->add_shape( e_primitive_shape::rect, Rect::create_centered( 6.f ), hit.pos );
					hit.scc->rs_opt.color = make_color( Color::teal );
				}
			}
		}
		break;

		// CLOSEST

		case e_input_id::gamepad_button_right_shoulder:
		case e_input_id::key_c:
		{
			reset_collision_trace_results();

			auto start = player->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::Raycast_Closest callback;
			sc_world->ray_cast( &callback, player, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<Primitve_Shape_Component>();
				ec->shapes.clear();
				ec->add_shape( e_primitive_shape::rect, Rect::create_centered( 6.f ), callback.result.pos );

				callback.result.scc->rs_opt.color = make_color( Color::teal );
			}
		}
		break;
	}

	return false;
}

bool Scene_Simple_Interact::on_input_motion( const Input_Event* evt )
{
	switch( evt->input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			player->add_force( { evt->delta, 10.f } );

			return true;
		}
		break;

		case e_input_id::gamepad_right_stick:
		{
			ray_dir = evt->delta;
			return true;
		}
		break;
	}

	return false;
}

