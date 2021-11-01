#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

Player_Entity::Player_Entity()
{
	tag = H( "player" );
	flags.include_in_quad_tree = true;

	fire_cooldown = Timer( 100 );

	{
		auto ec = add_component<Collision_Body_Component>();
		ec->set_as_centered_box( 12, 20 );
		ec->set_collision_flags( coll_flags.player, coll_flags.world );
	}
	{
		auto ec = add_component<Sprite_Component>();
		ec->init( "anim_player_walk_down" );
		sprite_component = ec;
	}

	snd_player_shoot = g_engine->find_asset<Sound_Asset>( "player_shoot" );
}

bool Player_Entity::can_fire_weapon()
{
	return fire_cooldown.is_elapsed();
}

void Player_Entity::fire_weapon( i32 angle )
{
	if( can_fire_weapon() )
	{
		fire_cooldown.restart();

		auto final_angle = snap_to_int( angle / 45.f );
		final_angle = snap_to_int( final_angle * 45.f );

		{
			auto e = parent_scene->add_entity<Player_Bullet_Entity>();
			e->set_pos( get_transform()->pos );
			e->set_angle( final_angle );

			e->add_impulse( 3.f );

			snd_player_shoot->play();
		}
	}
}

void Player_Entity::pre_update()
{

	sprite_component->init( "player_walk_down_1" );
	Entity::pre_update();
}

// ----------------------------------------------------------------------------

Player_Bullet_Entity::Player_Bullet_Entity()
{
	tag = H( "player_bullet" );
	flags.include_in_quad_tree = true;

	{
		auto ec = add_component<Collision_Body_Component>();
		ec->set_as_circle( 3 );
		ec->set_collision_flags( coll_flags.player_bullet, coll_flags.world );
	}
	{
		auto ec = add_component<Sprite_Component>();
		ec->init( "particle_simple_01" );
		ec->get_transform()->set_scale( 0.75f );
		ec->rs_opt.color = make_color( Color::teal );
	}
}

// ----------------------------------------------------------------------------

Enemy_Drone_Entity::Enemy_Drone_Entity()
{
	tag = H( "enemy_drone" );
	flags.include_in_quad_tree = true;

	{
		auto ec = add_component<Collision_Body_Component>();
		ec->set_as_centered_box( 12, 12 );
		ec->set_collision_flags( coll_flags.enemy_drone, coll_flags.enemy_drone | coll_flags.world | coll_flags.player | coll_flags.player_bullet );
	}
	{
		auto ec = add_component<Sprite_Component>();
		ec->init( "anim_enemy_drone_walk" );
	}

	next_move_timer = Timer( 150 );
	next_find_new_target_timer = Timer( 5000, b_start_elapsed );
}

void Enemy_Drone_Entity::update()
{
	Entity::update();

	if( next_find_new_target_timer.is_elapsed() )
	{
		next_find_new_target_timer.restart();
		find_new_target();
	}

	if( next_move_timer.is_elapsed() )
	{
		next_move_timer.restart();
		move_towards_target();
	}
}

void Enemy_Drone_Entity::find_new_target()
{
	target = parent_scene->find_entity( H( "player" ) );
	assert( target );
}

void Enemy_Drone_Entity::move_towards_target()
{
	assert( target );

	auto player = parent_scene->find_entity( H( "player" ) );
	assert( player );

	auto dir = ( player->get_pos() - get_pos() ).normalize();
	add_delta_pos( dir );
}
