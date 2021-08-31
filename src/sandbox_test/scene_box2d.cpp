
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static Bit_Flag_Generator collision_bits = 1;

static const unsigned scene_box2d_all = collision_bits.get();
static const unsigned scene_box2d_world = collision_bits.next();
static const unsigned scene_box2d_ball = collision_bits.next();

// ----------------------------------------------------------------------------

Scene_Box2D::Scene_Box2D()
{
	flags.blocks_further_drawing = true;
	flags.is_debug_physics_scene = true;
}

void Scene_Box2D::pushed()
{
	Scene::pushed();

	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	// kinematic circle
	{
		auto e = add_entity<Entity>();
		e->tag = H( "main_ball" );
		{
			{
				auto ec = e->add_component<Box2D_Kinematic_Body_Component>();
				ec->add_fixture_circle( hash_none, { 0.f, 0.f }, 32.f );
				ec->set_collision_flags( scene_box2d_world, scene_box2d_ball );
			}
			{
				auto ec = e->add_component<Primitve_Shape_Component>();
				ec->add_shape( e_primitive_shape::filled_circle, 32.f );
				ec->rs_opt.color = make_color( e_pal::brighter );
				ec->rs_opt.glow = 1.f;
			}
		}
	}

	// static world geometry
	{
		auto e = add_entity<Entity>();
		e->tag = H( "world" );
		{
			Rect rc_floor = { -viewport_hw, 0.f, viewport_w, 4.f };
			Rect rc_left_wall = { -viewport_hw, -viewport_hh * 6.f, 8.f, viewport_hh * 6.f };
			Rect rc_right_wall = { viewport_hw - 8.f, -viewport_hh * 6.f, 8.f, viewport_hh * 6.f };
			{
				auto ec = e->add_component<Box2D_Static_Body_Component>();
				ec->add_fixture_box( hash_none, rc_floor );
				ec->add_fixture_box( hash_none, rc_left_wall );
				ec->add_fixture_box( hash_none, rc_right_wall );
				e->get_component<Box2D_Physics_Component>()->set_collision_flags( scene_box2d_world, scene_box2d_ball );
			}
			{
				auto ec = e->add_component<Primitve_Shape_Component>();
				ec->add_shape( e_primitive_shape::filled_rect, rc_floor );
				ec->add_shape( e_primitive_shape::filled_rect, rc_left_wall );
				ec->add_shape( e_primitive_shape::filled_rect, rc_right_wall );
				ec->rs_opt.color = make_color( e_pal::brighter );
			}
		}
	}
}

void Scene_Box2D::draw()
{
	{
		scoped_render_state;

		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ), Rect( -viewport_hw, -viewport_h, viewport_w, viewport_h ) );
	}

	Scene::draw();
	Render::draw_world_axis();
}

void Scene_Box2D::draw_ui()
{
	Scene::draw_ui();
	draw_title( "Box2D Physics" );
}

// spawns a randomly sized emoji ball at "world_pos"

void Scene_Box2D::spawn_ball_at( Vec2 world_pos )
{
	auto e = add_entity<E_Emoji_Ball>();
	e->set_pos( world_pos );
	e->rs_opt.color = Color( Random::getf(), Random::getf(), Random::getf() );
	e->make_pickable();
	{
		float random_radius = Random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<Box2D_Dynamic_Body_Component>();
			ec->is_primary_body = true;
			ec->add_fixture_circle( hash_none, Vec2::zero, random_radius );
			ec->set_collision_flags( scene_box2d_ball, scene_box2d_ball | scene_box2d_world );
			e->get_component<Box2D_Physics_Component>()->set_restitution( Random::getf_range( 0.f, 1.f ) );
		}
		{
			auto ec = e->add_component<Sprite_Component>();
			ec->init( std::format( "emoji_{}", Random::geti_range( 1, 5 ) ) );
			ec->get_transform()->set_scale( random_radius / 8.f );
			ec->rs_opt.glow = Random::getb() ? Random::getf_range( 0.f, 2.f ) : 0.f;
		}
	}
}

// spawns a randomly sized and colored box at "world_pos"

void Scene_Box2D::spawn_box_at( Vec2 world_pos )
{
	float base_size = 8.f;
	float w = Random::getf_range( base_size, base_size * 10.f );
	float h = ( base_size * 15.f ) - w;
	Rect rc_box = { 0.f, 0.f, w, h };

	auto e = add_entity<Entity>();
	e->set_pos( world_pos );
	e->make_pickable();
	{
		float random_radius = Random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<Box2D_Dynamic_Body_Component>();
			ec->is_primary_body = true;
			ec->add_fixture_box( hash_none, rc_box );
			ec->set_collision_flags( scene_box2d_ball, scene_box2d_ball | scene_box2d_world );
			e->get_component<Box2D_Physics_Component>()->set_restitution( Random::getf_range( 0.f, 1.f ) );
		}
		rc_box.grow( 1.f );
		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, rc_box );
			ec->rs_opt.color = make_color( e_pal::darker );
		}
		rc_box.shrink( 2.f );
		{
			auto ec = e->add_component<Primitve_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, rc_box );

			glm::vec3 clr = { Random::getf(), Random::getf(), Random::getf() };
			auto sz = glm::length( clr );
			ec->rs_opt.color = Color( clr.r / sz, clr.g / sz, clr.b / sz );
			ec->rs_opt.glow = Random::getb() ? Random::getf_range( 0.f, 2.f ) : 0.f;
		}
	}
}

bool Scene_Box2D::on_input_pressed( const Input_Event* evt )
{
	// spawn ball at random location
	if( evt->input_id == e_input_id::key_r )
	{
		auto num_new_balls = 1;

		if( evt->shift_down )
		{
			num_new_balls = 20;
		}

		Range<float> spawn_x( -viewport_hw + 8, +viewport_hw - 8 );
		Range<float> spawn_y( -viewport_hh, -8.f );

		for( auto x = 0 ; x < num_new_balls ; ++x )
		{
			spawn_ball_at( { spawn_x.get_random_value(), spawn_y.get_random_value() } );
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

bool Scene_Box2D::on_input_motion( const Input_Event* evt )
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
