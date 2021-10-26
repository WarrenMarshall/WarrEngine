#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

E_Player::E_Player()
{
	tag = H( "player" );

	fire_cooldown = Timer( 100 );
	//fire_cooldown = Timer( 1000 );

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

bool E_Player::can_fire_weapon()
{
	return fire_cooldown.is_elapsed();
}

void E_Player::fire_weapon( i32 angle )
{
	if( can_fire_weapon() )
	{
		fire_cooldown.restart();

		auto final_angle = snap_to_int( angle / 45.f );
		final_angle = snap_to_int( final_angle * 45.f );

		{
			auto e = parent_scene->add_entity<E_Player_Bullet>();
			e->set_pos( get_transform()->pos );
			e->set_angle( final_angle );

			e->add_impulse( 3.f );

			snd_player_shoot->play();
		}
	}
}

void E_Player::pre_update()
{

	sprite_component->init( "player_walk_down_1" );
	Entity::pre_update();
}

// ----------------------------------------------------------------------------

E_Player_Bullet::E_Player_Bullet()
{
	tag = H( "player_bullet" );

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
