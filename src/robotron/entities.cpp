#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

E_Player::E_Player()
{
	tag = H( "player" );

	fire_cooldown = Timer( 100 );

	{
		auto ec = add_component<Simple_Collision_Body>();
		ec->set_as_centered_box( 8, 16 );
		ec->flags.draw_as_shape = true;
		ec->set_collision_flags( coll_flags.player, coll_flags.world );
	}
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
		}
	}
}

// ----------------------------------------------------------------------------

E_Player_Bullet::E_Player_Bullet()
{
	tag = H( "player_bullet" );

	{
		auto ec = add_component<Simple_Collision_Body>();
		ec->set_as_circle( 6 );
		ec->set_collision_flags( coll_flags.player_bullet, coll_flags.world );
	}
	{
		auto ec = add_component<Primitive_Shape_Component>();
		ec->add_shape( e_primitive_shape::point );
		ec->rs_opt.color = make_color( Color::teal );
		ec->rs_opt.glow = 3.f;
	}
}
