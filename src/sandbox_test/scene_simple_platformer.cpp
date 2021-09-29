
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

constexpr auto player_collision_radius = 8.f;

Scene_Simple_Platformer::Scene_Simple_Platformer()
{
	flags.is_debug_physics_scene = true;
	flags.requires_controller = true;
	flags.blocks_further_drawing = true;
	flags.blocks_further_update = true;
}

void Scene_Simple_Platformer::draw()
{
	Render::draw_world_axis();

	Scene::draw();
}

void Scene_Simple_Platformer::draw_ui()
{
	Render::draw_string( std::format( "{}", player->simple.is_in_air ), { 4, 4 } );
}

f_decl_tile_map_spawn_entity( platformer_spawn_entity )
{
	auto gameplay_scene = (Scene_Simple_Platformer*)scene;
	auto tmc = gameplay_scene->world->get_component<Tile_Map_Component>();

	Vec2 tile_pos =
	{
		( ( tile->x_idx + chunk->tilemap_bounds.x ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f ),
		( ( tile->y_idx + chunk->tilemap_bounds.y ) * tile_map->tile_sz ) + ( tile_map->tile_sz / 2.f )
	};

	switch( tile->idx )
	{
		// jump pad

		case 107:
		{
			auto e = scene->add_entity<E_Jump_Pad>();
			e->tag = H( "JUMP_PAD" );
			e->set_pos( tile_pos );

			{
				auto ec = e->add_component<Sprite_Component>();
				ec->init( "tex_jump_pad_down" );

				e->sprite_component = ec;
			}
			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->solidity = e_solidity::sensor;
				ec->set_sensor_as_continuous();
				ec->set_as_centered_box( 16.f, 6.f );
				ec->get_transform()->add_pos( { 0.f, 4.f } );

				ec->set_collision_flags(
					gameplay_scene->coll_flags.jump_pad,
					0
				);
			}
		}
		break;

		// player

		case 145:
		{
			auto e = scene->add_entity<E_Player>();
			e->set_pos( tile_pos );
			e->simple.is_affected_by_gravity = true;
			e->simple.set_friction( 5.0f );

			{
				auto ec = e->add_component<Sprite_Component>();
				ec->init( "tex_player" );
			}
			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->tag = H( "player_body" );
				ec->set_as_circle( player_collision_radius );

				ec->set_collision_flags(
					gameplay_scene->coll_flags.player,
					gameplay_scene->coll_flags.geo | gameplay_scene->coll_flags.jump_pad
				);
			}
			{
				auto ec = e->add_component<Simple_Collision_Body>();
				ec->tag = H( "ground_sensor" );
				ec->solidity = e_solidity::sensor;
				ec->set_sensor_as_continuous();
				ec->set_as_circle( 4.f );
				ec->get_transform()->set_pos( { 0.f, 8.f } );

				ec->set_collision_flags(
					gameplay_scene->coll_flags.player,
					gameplay_scene->coll_flags.geo
				);
			}

			gameplay_scene->player = e;
		}
		break;
	}
}

void Scene_Simple_Platformer::pushed()
{
	Scene::pushed();

	viewport_pivot = { viewport_hw, viewport_hh };
	get_transform()->set_scale( 2.0f );

	g_engine->render.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	fx_red_alert.clear( 1000 );
	fx_red_alert.add_kf_shake_angle( true, 0.0f, 1000, get_transform(), 2.0f );
	fx_red_alert.add_kf_play_sound( true, 0.0f, g_engine->find_asset<Sound_Asset>( "sfx_platformer_boom" ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.0f, 250, make_color( Color::red, 0.25f ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.3f, 250, make_color( Color::red, 0.5f ) );
	fx_red_alert.add_kf_pp_color_overlay( true, 0.6f, 250, make_color( Color::red, 0.25f ) );

	// world

	{
		world = add_entity<Entity>();
		world->simple.type = e_sc_type::stationary;

		{
			auto ec = world->add_component<Tile_Map_Component>();
			ec->set_collision_flags( coll_flags.geo, 0 );
			ec->init( "ts_platformer", "tm_platformer_level_01" );
			ec->spawn_entities( this, platformer_spawn_entity );
		}
	}

	// mover

	{
		auto e = add_entity<Entity>();
		e->set_pos( { 0.f, -8.f } );
		e->simple.type = e_sc_type::kinematic;

	#if 1
		// flying rectangle with platform collision
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, Rect( -24.f, -8.f, 48.f, 16.f ) );
			ec->rs_opt.color = make_color( Color::teal, 0.25f );
		}
		{
			auto ec = e->add_component<Simple_Collision_Platform_Body>();
			ec->set_as_centered_box( 48.f, 1.f );
			ec->get_transform()->add_pos( { 0.f, -8.0f } );

			ec->set_collision_flags(
				coll_flags.geo,
				0
			);
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->solidity = e_solidity::sensor;
			ec->set_sensor_as_continuous();
			ec->set_as_centered_box( 48.f, 4.f );
			ec->get_transform()->add_pos( { 0.f, -8.0f } );
			ec->is_sticky = true;

			ec->set_collision_flags(
				coll_flags.geo,
				0
			);
		}
	#else
		// flying cube, regular collision
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			ec->add_shape( e_primitive_shape::filled_rect, Rect( -16.f, -16.f, 32.f, 32.f ) );
			ec->rs_opt.color = make_color( Color::teal, 0.25f );
		}
		{
			//auto ec = e->add_component<Simple_Collision_Platform_Body>();
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->set_as_centered_box( 32.f, 32.f );
			ec->get_transform()->add_pos( { 0.f, -0.0f } );

			ec->set_collision_flags(
				coll_flags.geo,
				0
			);
		}
		{
			auto ec = e->add_component<Simple_Collision_Body>();
			ec->collider_type = e_solidity::sensor;
			ec->set_sensor_as_continuous();
			//ec->set_as_centered_box( 48.f, 4.f );
			//ec->get_transform()->add_pos( { 0.f, -8.0f } );
			ec->set_as_centered_box( 34.f, 34.f );
			ec->get_transform()->add_pos( { 0.f, -0.0f } );
			ec->is_sticky = true;

			ec->set_collision_flags(
				coll_flags.geo,
				0
			);
		}

	#endif

		mover = e;
	}
}

void Scene_Simple_Platformer::update()
{
	Scene::update();

	fx_red_alert.update();

	follow_cam( player->get_transform() );

	// keyboard input

	if( g_engine->input_mgr.is_button_down( e_input_id::key_left ) )
	{
		player->apply_movement_walk( -Vec2::x_axis, 150.f );
	}
	if( g_engine->input_mgr.is_button_down( e_input_id::key_right ) )
	{
		player->apply_movement_walk( Vec2::x_axis, 150.f );
	}
}

bool Scene_Simple_Platformer::on_input_motion( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::gamepad_left_stick )
	{
		player->apply_movement_walk( evt->delta, 12.f );
	}
	if( evt->input_id == e_input_id::gamepad_right_stick )
	{
		mover->add_delta_pos( evt->delta );
	}

	return false;
}

bool Scene_Simple_Platformer::on_input_pressed( const Input_Event* evt )
{
	if( evt->input_id == e_input_id::gamepad_button_a
		or evt->input_id == e_input_id::key_space )
	{

		if( !player->simple.is_in_air
			and g_engine->input_mgr.get_axis_state( e_input_id::gamepad_left_stick, true ).y > 0.5f )
		{
			player->add_delta_pos( Vec2::y_axis * player_collision_radius );
			player->velocity.x = 0.f;
		}
		else
		{
			player->apply_movement_jump();
		}
	}

	if( evt->input_id == e_input_id::gamepad_button_x )
	{
		if( !fx_red_alert.life_cycle.is_alive() )
		{
			fx_red_alert.go();
		}
	}

	return false;
}

// ----------------------------------------------------------------------------

bool E_Player::on_touching_begin( Simple_Collision_Body* sensor )
{
	Entity::on_touching_begin( sensor );

	if( sensor->parent_entity->tag == H("JUMP_PAD") )
	{
		g_engine->find_asset<Sound_Asset>( "sfx_platformer_jump" )->play();
		add_impulse( { Vec2( 0.0f, -1.0f ), 50.0f } );
		( (E_Jump_Pad*)sensor->parent_entity )->time_reset = g_engine->clock.now() + 500;
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------

E_Jump_Pad::E_Jump_Pad()
	: Entity()
{
	tex_jump_pad_down = g_engine->find_asset<Texture_Asset>( "tex_jump_pad_down" );
	tex_jump_pad_up = g_engine->find_asset<Texture_Asset>( "tex_jump_pad_up" );

}

void E_Jump_Pad::update()
{
	if( time_reset < g_engine->clock.now() )
	{
		sprite_component->texture = tex_jump_pad_down;
	}
	else
	{
		sprite_component->texture = tex_jump_pad_up;
	}
}
