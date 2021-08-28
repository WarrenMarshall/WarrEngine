
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static Bit_Flag_Generator collision_bits = 1;

static const unsigned scene_simple_platformer_coll_player = collision_bits.get();
static const unsigned scene_simple_platformer_coll_geo = collision_bits.next();

constexpr auto player_collision_radius = 7.f;

// ----------------------------------------------------------------------------

scene_simple_platformer::scene_simple_platformer()
{
	flags.is_debug_physics_scene = true;
	flags.requires_controller = true;
}

void scene_simple_platformer::draw_ui()
{
	scene::draw_ui();

	//render::draw_string( std::format( "Vel : {:.1f}, {:.1f} / In-Air : {}",
	//	player->velocity.x, player->velocity.y, player->simple.is_in_air ),
	//	vec2( 8.f, 8.f ) );
}

void scene_simple_platformer::draw()
{
	{
		scoped_render_state;
		Render::state->color = make_color( pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
}

f_decl_tile_map_spawn_entity( platformer_spawn_entity )
{
	auto gameplay_scene = (scene_simple_platformer*)scene;

	switch( tile->idx )
	{
		case 15:
		{
			auto tmc = gameplay_scene->world->get_component<Tile_Map_Component>();

			auto e = scene->add_entity<Entity>();
			e->set_pos( Vec2( tile->x_idx * tmc->tile_map->tile_sz, tile->y_idx * tmc->tile_map->tile_sz ) );
			e->add_delta_pos( Vec2( tmc->tile_map->tile_sz / 2.f, tmc->tile_map->tile_sz / 2.f ) );
			e->simple.is_affected_by_gravity = true;
			e->simple.friction = 0.1f;

			{
				auto ec = e->add_component<Primitve_Shape_Component>();
				ec->add_shape( primitive_shape::point );
			}
			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->tag = H( "player_body" );
				ec->set_as_circle( player_collision_radius );

				ec->set_collision_flags( scene_simple_platformer_coll_player, scene_simple_platformer_coll_geo );
			}
			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->tag = H( "ground_sensor" );
				ec->set_body_collider_type( sc_body_collider_type::sensor );
				ec->set_as_circle( 4.f );
				ec->get_transform()->set_pos( { 0.f, 8.f } );

				ec->set_collision_flags( scene_simple_platformer_coll_player, scene_simple_platformer_coll_geo );
			}
			{
				auto ec = e->add_component<Emitter_Component>();
				ec->init( "em_stars" );
				ec->get_transform()->set_scale( 0.25f );
			}

			gameplay_scene->player = e;

			tile->idx = Tile_Map_Asset::Tile::empty;
		}
		break;
	}
}

void scene_simple_platformer::pushed()
{
	scene::pushed();

	viewport_pivot = { viewport_hw, viewport_hh };
	get_transform()->set_scale( 2.0f );

	g_engine->renderer.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( mouse_mode::os );

	fx_red_alert.clear( 1000 );
	fx_red_alert.add_kf_shake_angle( true, 0.0f, 1000, get_transform(), 2.0f );
	fx_red_alert.add_kf_play_sound( true, 0.0f, g_engine->find_asset<Sound_Asset>( "sfx_platfomer_boom" ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.0f, 250, make_color( Color::red, 0.25f ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.3f, 250, make_color( Color::red, 0.5f ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.6f, 250, make_color( Color::red, 0.25f ) );

	// world

	{
		world = add_entity<Entity>();
		world->simple.type = sc_type::stationary;

		{
			auto ec = world->add_component<Tile_Map_Component>();
			ec->set_collision_flags( scene_simple_platformer_coll_geo, 0 );
			ec->init( "ts_platformer", "tm_platformer_level_01" );
			ec->spawn_entities( this, platformer_spawn_entity );
		}
	}
}

void scene_simple_platformer::update()
{
	scene::update();

	fx_red_alert.update();

	follow_cam( player->get_transform());

	// keyboard input

	if( g_engine->input.is_button_down( input_id::key_left ) )
	{
		player->apply_movement_walk( -Vec2::x_axis, 150.f );
	}
	if( g_engine->input.is_button_down( input_id::key_right ) )
	{
		player->apply_movement_walk( Vec2::x_axis, 150.f );
	}
}

bool scene_simple_platformer::on_input_motion( const Input_Event* evt )
{
	if( evt->input_id == input_id::gamepad_left_stick )
	{
		player->apply_movement_walk( evt->delta, 12.f );
	}

	return false;
}

bool scene_simple_platformer::on_input_pressed( const Input_Event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a
		or evt->input_id == input_id::key_space )
	{

		if( !player->simple.is_in_air
			and g_engine->input.get_axis_state( input_id::gamepad_left_stick, true ).y > 0.5f )
		{
			player->add_delta_pos( Vec2::y_axis * player_collision_radius );
			player->velocity.x = 0.f;
		}
		else
		{
			player->apply_movement_jump();
		}
	}

	if( evt->input_id == input_id::gamepad_button_x )
	{
		if( !fx_red_alert.life_cycle.is_alive() )
		{
			fx_red_alert.go();
		}
	}

	return false;
}
