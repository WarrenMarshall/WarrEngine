
#include "app_header.h"

e_platformer_player::e_platformer_player()
{
	set_collision( clayer_player, clayer_world );
	set_transform( { 32.0f, 0.0f }, 0, 1 );
	draw_debug_info = true;

	auto ec = add_component<ec_b2d_dynamic>();
	{
		ec->body->SetFixedRotation( true );

		ec->add_fixture_polygon(
			sensor_id::none,
			w_vec2( 0.0f, 0.0f ),
			{
				{ -8, -8 },
				{ 8, -8 },
				{ 8, 2 },
				{ 5, 8 },
				{ -5, 8 },
				{ -8, 2 }
			}
		);

		ec->add_fixture_box( sensor_id::ground, { 0.0f, 8.0f }, 12.0f, 4.0f )->SetSensor( true );
		ec->add_fixture_box( sensor_id::area_01, { 0.0f, 20.0f }, 12.0f, 16.0f )->SetSensor( true );
	}

	add_component<ec_sprite>()->init( "sprite_mario" );
}
