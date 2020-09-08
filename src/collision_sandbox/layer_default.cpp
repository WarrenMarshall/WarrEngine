
#include "app_header.h"

layer_default::layer_default()
{
	hit.count = 0;
	draws_completely_solid = true;
}

void layer_default::push()
{
	player = add_entity<w_entity>();
#if 0
	player_ec = static_cast<ec_collider*>( player->add_component<ec_collider>()->init_as_box( { -12,-12,24,24 } ) );
#else
	player_ec = static_cast<ec_collider*>( player->add_component<ec_collider>()->init_as_circle( 24 ) );
#endif
	player->set_transform( { v_window_hw, v_window_hh }, 0, 1.0f );
	player->debug_draw_collision = true;

	blocker = add_entity<w_entity>();
#if 0
	blocker_ec = static_cast<ec_collider*>( blocker_circle->add_component<ec_collider>()->init_as_box( { 0,0, 200, 32 } ) );
	blocker_circle->set_transform( { 64.0f, 64.0f }, 0, 1 );
#else
	blocker_ec = static_cast<ec_collider*>( blocker->add_component<ec_collider>()->init_as_circle( { 32 } ) );
	blocker->set_transform( { v_window_hw, 64.0f }, 0, 1.0f );
#endif
	blocker->debug_draw_collision = true;
}

void layer_default::update()
{
	static float player_speed = 50.0f;

	w_vec2 delta = engine->input->axis_value_of( input_id::controller_left_stick );

	if( delta.x != 0.0f || delta.y != 0.0f )
	{
		player->set_pos(
			{
				player->pos.x + ( ( delta.x * player_speed ) * engine->time->FTS_step_value_s ),
				player->pos.y + ( ( delta.y * player_speed ) * engine->time->FTS_step_value_s )
			}
		);
	}

	// ----------------------------------------------------------------------------

	status = "";

	c2Collide(
		&player_ec->get_collider(), NULL, player_ec->c2type,
		&blocker_ec->get_collider(), NULL, blocker_ec->c2type,
		&hit );

	if( hit.count )
	{
		status = fmt::format( "h:{}, d:{:.2f}, n:{:.2f},{:.2f}", hit.count, hit.depths[ 0 ], hit.n.x, hit.n.y );
	}
}

void layer_default::draw()
{
	w_layer::draw();

	if( hit.count )
	{
		RENDER
			->push_rgb( w_color::white )
			->draw_line( player->pos, { hit.contact_points->x, hit.contact_points->y } )
			->push_rgb( w_color::red )
			->draw_point( { hit.contact_points[ 0 ].x, hit.contact_points[ 0 ].y } )
			->pop_rgb()
			->draw_string( engine->pixel_font, status, w_rect( player->pos.x, player->pos.y ) );
	}
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_released)
	{
		if( evt->input_id == input_id::controller_button_b )
		{
			if( hit.count )
			{
				w_vec2 push_delta = w_vec2::multiply( w_vec2( hit.n.x, hit.n.y ), -1.0f * (hit.depths[ 0 ] + 0.5f) );
				player->set_pos( player->pos.add( push_delta ) );

				hit.count = 0;
			}

			return true;
		}
	}

	return false;
}
