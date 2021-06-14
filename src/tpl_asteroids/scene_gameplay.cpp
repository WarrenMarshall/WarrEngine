
#include "app_header.h"

using namespace war;

scene_gameplay::scene_gameplay()
{
	flags.is_debug_physics_scene = true;

	ship_power_progress.draw_percentage_as_text = false;

	ship_power_timer = timer( my_game->get_ship_powerup_timer_duration() );
}

void scene_gameplay::draw_ui()
{
	scene::draw_ui();

	if( !my_game->ship_is_at_max_level() )
	{
/*
		g_ui
			->do_control<ui_progress_control>( H( "ship_power" ) )
			->set_rect( { 2.f, ui_h - 16.f, ui_w - 4.f, 8.f } )
			->finalize( &ship_power_progress );
*/
	}

	render::state->color = make_color( color::dark_grey, 0.2f );
	render::state->scale = 6.0f;
	render::state->align = align::centered;
	render::draw_string( f_commas( (float)my_game->player_score ), { ui_hw, ui_hh } );
}

void scene_gameplay::pushed()
{
	viewport_pivot = { viewport_hw, viewport_hh };

	g_engine->window.set_mouse_mode( mouse_mode::os );

	player = add_entity<e_player>();

	for( auto idx = 0 ; idx < 9 ; ++idx )
	{
		player->satellite_ships[idx] = add_entity<e_player_ship>();
	}
}

void scene_gameplay::update()
{
	scene::update();

	auto lstick = g_engine->input.get_axis_state( input_id::gamepad_left_stick, true );

	vec2 new_player_pos = player->get_transform()->pos + ( fixed_time_step::per_second( lstick * my_game->ship_base_movement_speed ) );

	if( new_player_pos.x < -viewport_hw )
	{
		new_player_pos.x += viewport_w;
	}
	else if( new_player_pos.x > viewport_hw )
	{
		new_player_pos.x -= viewport_w;
	}
	else if( new_player_pos.y < -viewport_hh )
	{
		new_player_pos.y += viewport_h;
	}
	else if( new_player_pos.y > viewport_hh )
	{
		new_player_pos.y -= viewport_h;
	}

	player->transform_set_pos( new_player_pos );

	vec2 offset_positions [] =
	{
		{ -1.f, -1.f }, { +0.f, -1.f }, { +1.f, -1.f },
		{ -1.f, +0.f }, { +0.f, +0.f }, { +1.f, +0.f },
		{ -1.f, +1.f }, { +0.f, +1.f }, { +1.f, +1.f },
	};

	for( auto idx = 0 ; idx < 9 ; ++idx )
	{
		player->satellite_ships[ idx ]->transform_set_pos(
			player->get_transform()->pos + ( offset_positions[ idx ] * viewport_w )
		);
	}

	if( !my_game->ship_is_at_max_level() )
	{
		ship_power_progress.set_float_value( ship_power_timer.get_pct_complete() );
		if( ship_power_timer.is_elapsed() )
		{
			my_game->level_up_player_ship();
			ship_power_timer = timer( my_game->get_ship_powerup_timer_duration() );
			ship_power_progress.set_float_value( 0.f );
		}
	}
}

bool scene_gameplay::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::gamepad_button_a )
	{
		player->fire();
		return true;
	}

	switch( evt->input_id )
	{
		case input_id::key_q:
		{

			return true;
		}

		case input_id::key_1: { my_game->ship_power_level = 0; return true; }
		case input_id::key_2: { my_game->ship_power_level = 1; return true; }
		case input_id::key_3: { my_game->ship_power_level = 2; return true; }
		case input_id::key_4: { my_game->ship_power_level = 3; return true; }
		case input_id::key_5: { my_game->ship_power_level = 4; return true; }
		case input_id::key_6: { my_game->ship_power_level = 5; return true; }
		case input_id::key_7: { my_game->ship_power_level = 6; return true; }
		case input_id::key_8: { my_game->ship_power_level = 7; return true; }
		case input_id::key_9: { my_game->ship_power_level = 8; return true; }
		case input_id::key_0: { my_game->ship_power_level = 9; return true; }
	}

	return false;
}
