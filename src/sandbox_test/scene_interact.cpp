
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

constexpr auto max_raycast_length = 350.f;
constexpr auto radius = 12.f;

// ----------------------------------------------------------------------------

Scene_Interact::Scene_Interact()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

Entity* Scene_Interact::spawn_entity()
{
	auto e = add_entity<Entity>();
	e->flags.include_in_quad_tree = true;
	e->set_pos( { 0.f, 0.f } );
	e->set_scale( 1.5f );
	{
		auto ec = e->add_component<Collision_Body_Component>();

		static i32 last_spawned_type = 0;
		last_spawned_type++;
		switch( last_spawned_type % 3 )
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

		ec->set_collision_flags( coll_flags.player, coll_flags.geo | coll_flags.player );
		ec->flags.draw_as_shape = true;
	}
	e->collision.flags.is_bouncy = false;

	return e;
}


void Scene_Interact::pushed()
{
	Scene::pushed();

	Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
	spatial_map.set_bounds( rc );
	spatial_map.min_node_area = 128;

#ifndef _RELEASE
		g_engine->render.debug.draw_colliders = true;
#endif

	// PLAYER

	player = spawn_entity();
	player->add_delta_pos( { 0.f, 80.f } );
	player->rs_opt.glow = 3.0f;	// player has a glow
	spawn_entity();

	// HIT MARKER

	{
		auto e = add_entity<Entity>();
		e->tag = H( "hit_marker" );
		e->set_pos( { 0.f, 0.f } );
		e->rs_opt.z_bias = zdepth_nudge;
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->rs_opt.color = make_color( Color::yellow );
		}

		hit_marker = e;

	}

	// GEO

	{
		int num_primitives = 4;

		auto e = add_entity<Entity>();
		e->flags.include_in_quad_tree = true;
		e->collision.type = e_physics_body_type::stationary;

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
				ec->flags.draw_as_shape = true;
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
				ec->flags.draw_as_shape = true;
			}
		}

		// 4 walls
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->flags.draw_as_shape = true;
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->flags.draw_as_shape = true;
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->flags.draw_as_shape = true;
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->flags.draw_as_shape = true;
		}

		world_geo = e;
	}
}

void Scene_Interact::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
		Render::flush();
	}

	spatial_map.debug_draw();

	Scene::draw();
	//render::draw_world_axis();

	if( b_show_ray )
	{
		Render::state->color = make_color( Color::green, 0.25f );
		auto start = player->get_pos();
		Render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void Scene_Interact::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Interact" );

	//render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
	//	player->velocity.x, player->velocity.y ), vec2( 8.f, 24.f ) );
}

void Scene_Interact::update()
{
	Scene::update();

	// show the raycast beam if the right stick is being pushed
	b_show_ray = g_engine->input_mgr.get_axis_state( e_input_id::gamepad_right_stick ).get_size_fast() > 0.f;

	follow_cam( player->get_transform() );
}

void Scene_Interact::reset_collision_trace_results()
{
	( ( Primitive_Shape_Component* )hit_marker->get_component( e_component_type::primitive_shape ) )->shapes.clear();

	for( auto& iter : world_geo->get_components( e_component_type::collision_body ) )
	{
		iter->rs_opt.color = make_color( Color::dark_teal );
	}
}

bool Scene_Interact::on_input( const Input_Event* evt )
{
	if( Scene::on_input( evt ) )
	{
		return true;
	}

	if( evt->is_pressed() )
	{
		switch( evt->input_id )
		{
			case e_input_id::key_1:
			{
				auto ec = ( Collision_Body_Component* )player->get_component( e_component_type::collision_body );
				ec->set_as_centered_box( radius * Random::getf_range( 0.5f, 3.0f ), radius * Random::getf_range( 0.5f, 3.0f ) );
			}
			break;

			case e_input_id::key_2:
			{
				auto ec = ( Collision_Body_Component* )player->get_component( e_component_type::collision_body );
				ec->set_as_circle( radius * Random::getf_range( 0.5f, 2.0f ) );
			}
			break;

			case e_input_id::key_3:
			{
				auto ec = ( Collision_Body_Component* )player->get_component( e_component_type::collision_body );

				auto s = Random::geti_range( 3, 8 );
				auto r = radius * Random::getf_range( 0.5f, 3.0f );
				ec->set_as_polygon( Geo_Util::generate_convex_shape( s, r ) );
			}
			break;

			case e_input_id::gamepad_button_y:
			case e_input_id::key_space:
			{
				spawn_entity();
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

				collision::Raycast_All callback;
				sc_world.ray_cast( &callback, player, start, end );

				if( callback.hit_something )
				{
					auto ec = ( Primitive_Shape_Component* )hit_marker->get_component( e_component_type::primitive_shape );
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

				collision::Raycast_Closest callback;
				sc_world.ray_cast( &callback, player, start, end );

				if( callback.hit_something )
				{
					auto ec = ( Primitive_Shape_Component* )hit_marker->get_component( e_component_type::primitive_shape );
					ec->shapes.clear();
					ec->add_shape( e_primitive_shape::rect, Rect::create_centered( 6.f ), callback.result.pos );

					callback.result.scc->rs_opt.color = make_color( Color::teal );
				}
			}
			break;
		}
	}
	else if( evt->is_motion() )
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
	}

	return false;
}
