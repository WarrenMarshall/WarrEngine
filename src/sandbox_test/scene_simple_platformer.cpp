
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

void Scene_Simple_Platformer::draw_ui()
{
	Scene::draw_ui();

	//render::draw_string( std::format( "Vel : {:.1f}, {:.1f} / In-Air : {}",
	//	player->velocity.x, player->velocity.y, player->simple.is_in_air ),
	//	vec2( 8.f, 8.f ) );
}

void Scene_Simple_Platformer::draw()
{
/*
	{
		scoped_render_state;
		Render::state->color = make_color( e_pal::darker );
		Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ),
			Rect( -viewport_w, -viewport_h, viewport_w*2, viewport_h*2 ) );
	}
*/

	Render::draw_world_axis();

	Scene::draw();
}

f_decl_tile_map_spawn_entity( platformer_spawn_entity )
{
	auto gameplay_scene = (Scene_Simple_Platformer*)scene;
	auto tmc = gameplay_scene->world->get_component<Tile_Map_Component>();

	Vec2 tile_pos =
	{
		( ( tile->x_idx + chunk->tilemap_bounds.x ) * tile_map->tile_sz )
			+ ( tile_map->tile_sz / 2.f ),
		( ( tile->y_idx + chunk->tilemap_bounds.y ) * tile_map->tile_sz )
			+ ( tile_map->tile_sz / 2.f )
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
				ec->collider_type = e_sc_body_collider_type::sensor;
				ec->set_as_centered_box( 16.f, 6.f );
				ec->get_transform()->add_pos( { 0.f, 4.f } );

				ec->set_collision_flags(
					gameplay_scene->coll_flags.jump_pad,
					0
				);
			}

			tile->idx = Tile_Map_Asset::Tile::empty;
		}
		break;

		// player

		case 145:
		{
			auto e = scene->add_entity<E_Player>();
			e->set_pos( tile_pos );
			e->simple.is_affected_by_gravity = true;
			e->simple.friction = 0.1f;

			{
				auto ec = e->add_component<Primitive_Shape_Component>();
				ec->add_shape( e_primitive_shape::point );
			}
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
				ec->set_body_collider_type( e_sc_body_collider_type::sensor );
				ec->set_as_circle( 4.f );
				ec->get_transform()->set_pos( { 0.f, 8.f } );

				ec->set_collision_flags(
					gameplay_scene->coll_flags.player,
					gameplay_scene->coll_flags.geo
				);
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

void Scene_Simple_Platformer::pushed()
{
	Scene::pushed();

	viewport_pivot = { viewport_hw, viewport_hh };
	get_transform()->set_scale( 2.0f );

	g_engine->render.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( e_mouse_mode::os );

	fx_red_alert.clear( 1000 );
	fx_red_alert.add_kf_shake_angle( true, 0.0f, 1000, get_transform(), 2.0f );
	fx_red_alert.add_kf_play_sound( true, 0.0f, g_engine->find_asset<Sound_Asset>( "sfx_platfomer_boom" ) );
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
}

void Scene_Simple_Platformer::update()
{
	Scene::update();

	fx_red_alert.update();

	follow_cam( player->get_transform());

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
	if( sensor->parent_entity->tag == H("JUMP_PAD") )
	{
		g_engine->find_asset<Sound_Asset>( "sfx_platfomer_jump" )->play();
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
