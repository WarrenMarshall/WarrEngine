
#include "app_header.h"

using namespace war;

Scene_Gameplay::Scene_Gameplay()
{
	flags.blocks_further_drawing = true;
	flags.blocks_further_input = true;
}

void Scene_Gameplay::pushed()
{
	Scene::pushed();

	//g_engine->window.push_mouse_mode( e_mouse_mode::locked );

	spatial_map.set_bounds( { -viewport_hw, -viewport_hh, viewport_w, viewport_h } );

	// ----------------------------------------------------------------------------
	// WORLD

	{
		auto e = add_entity<Entity>();
		e->tag = H( "world_geo" );
		e->collision.type = e_physics_body_type::stationary;
		e->flags.include_in_quad_tree = true;

		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 16.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh + 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Collision_Body_Component>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( coll_flags.world, 0 );
		}
		{
			auto ec = e->add_component<Primitive_Shape_Component>();
			Rect rc( -viewport_hw, -viewport_hh, viewport_w, viewport_h );
			rc += Rect( 8.f, 24.f, -16.f, -40.f );
			ec->add_shape( e_primitive_shape::rect, rc );
			ec->rs_opt.color = make_color( Color::orange );
			ec->rs_opt.glow = 3.f;
		}
	}

	// ----------------------------------------------------------------------------
	// PLAYER

	player = add_entity<E_Player>();

	// ----------------------------------------------------------------------------
	// RANDO ENEMIES

/*
	for( auto x = 0 ; x < 16 ; ++x )
	{
		spawn_random_drone();
	}
*/

}

void Scene_Gameplay::spawn_random_drone()
{
	auto e = add_entity<E_Enemy_Drone>();
	auto pos = Random::get_random_in_circle( 1.f );
	pos.x *= viewport_hw / 2.f;
	pos.y *= viewport_hh / 2.f;
	e->set_pos( pos );
}

void Scene_Gameplay::popped()
{
	Scene::popped();

	//g_engine->window.pop_mouse_mode();
}

void Scene_Gameplay::draw()
{
	Scene::draw();

}

bool Scene_Gameplay::on_input_pressed( const Input_Event* evt )
{
	if( Scene::on_input_pressed( evt ) )
	{
		return true;
	}

	switch( evt->input_id )
	{
		case e_input_id::mouse_button_left:
		{
			auto e = add_entity<E_Enemy_Drone>();
			e->set_pos( Coord_System::window_to_world_pos( evt->mouse_pos ) );

			return true;
		}


		case e_input_id::gamepad_button_y:
		{
			spawn_random_drone();
			return true;
		}
	}
	return false;
}

bool Scene_Gameplay::on_input_motion( const Input_Event* evt )
{
	if( Scene::on_input_motion( evt ) )
	{
		return true;
	}

	switch( evt->input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			Vec2 movement = Vec2::snap_to_int( evt->delta );
			if( !movement.is_zero() )
			{
				movement.normalize();
				player->add_delta_pos( movement );

				if( glm::abs( evt->delta.x ) > glm::abs( evt->delta.y ) )
				{
					if( glm::sign( evt->delta.x ) > 0.f )
					{
						player->sprite_component->init( "anim_player_walk_right" );
					}
					else
					{
						player->sprite_component->init( "anim_player_walk_left" );
					}
				}
				else
				{
					if( glm::sign( evt->delta.y ) < 0.f )
					{
						player->sprite_component->init( "anim_player_walk_up" );
					}
					else
					{
						player->sprite_component->init( "anim_player_walk_down" );
					}
				}
			}
			return true;
		}

		case e_input_id::gamepad_right_stick:
		{
			player->fire_weapon( ( i32 )Vec2::dir_to_angle( evt->delta ) );

			return true;
		}
	}

	return false;
}

[[nodiscard]] Entity* find_entity_in_map( std::map<hash, Entity*>& map, hash tag )
{
	auto iter = map.find( tag );
	return ( iter == map.end() ) ? nullptr : iter->second;
}

bool Scene_Gameplay::on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll )
{
	std::map<hash, Entity*> map;
	map.insert( std::make_pair( entity_a->tag, entity_a ) );
	map.insert( std::make_pair( entity_b->tag, entity_b ) );

	auto world_geo = find_entity_in_map( map, H( "world_geo" ) );
	auto player_bullet = find_entity_in_map( map, H( "player_bullet" ) );
	auto enemy_drone = find_entity_in_map( map, H( "enemy_drone" ) );

	if( player_bullet and world_geo )
	{
		player_bullet->set_life_cycle( e_life_cycle::dying );

		{
			auto e = add_entity<Entity_Transient>();
			e->set_pos( entity_a->get_transform()->pos );
			{
				auto ec = e->add_component<Emitter_Component>();
				ec->init( "em_player_bullet_hit_world_geo" );
			}
			{
				auto ec = e->add_component<Sound_Component>();
				ec->init( "player_bullet_hit_world", true, true );
			}
		}

		return true;
	}

	if( player_bullet and enemy_drone )
	{
		player_bullet->set_life_cycle( e_life_cycle::dying );
		enemy_drone->set_life_cycle( e_life_cycle::dying );

		{
			auto e = add_entity<Entity_Transient>();
			e->set_pos( entity_a->get_transform()->pos );
			{
				auto ec = e->add_component<Emitter_Component>();
				ec->init( "em_player_bullet_hit_enemy_drone" );
			}
			{
				auto ec = e->add_component<Sound_Component>();
				ec->init( "player_bullet_hit_enemy_drone", true, true );
			}
		}

		return true;
	}

	return false;
}
