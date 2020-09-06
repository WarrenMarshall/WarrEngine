
#include "app_header.h"

constexpr int multiball_spawn_interval = 4000;
constexpr int fireball_spawn_interval = 3;

void spawn_new_brick_row( layer_gameplay* layer )
{
	static int spawned_brick_counter = 0;

	int color_idx = w_random::geti_range( 0, static_cast<int>( GAME->brick_textures.size() - 1 ) );
	std::string subtex_name = GAME->brick_textures[ color_idx ]->name;

	int xpos = 24 + w_random::geti_range( 0, 64 );
	for( int x = 0 ; x < 5 ; ++x )
	{
		if( w_random::geti_range( 0, 3 ) != 0 )
		{
			spawned_brick_counter++;

			if( !(spawned_brick_counter % multiball_spawn_interval) )
			{
				layer->add_entity<e_brick_multiball>( { xpos, -16 }, 0.0f, 0.75f );
			}
			else if( !( spawned_brick_counter % fireball_spawn_interval ) )
			{
				layer->add_entity<e_brick_fireball>( { xpos, -16 }, 0.0f, 0.75f );
			}
			else
			{
				auto brick = layer->add_entity<e_brick>( { xpos, -16 }, w_random::getf_range( -3.0f, 3.0f ), 0.75f );
				brick->add_component<ec_sprite>()->init( subtex_name );
			}
		}
		xpos += 48;
	}
}

layer_gameplay::layer_gameplay()
{
}

void layer_gameplay::push()
{
	GAME->mus_game->play();

	timer_brick_move = std::make_unique<w_timer>( 250 );

	ball = add_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.625f );

	auto walls = add_entity<w_entity>();

	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, 1.0f, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( v_window_w - 1.0f, 0.0f, v_window_w, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, v_window_w, 1.0f ) );
	walls->collision_layer = cl_wall;

	auto death_zone = add_entity<w_entity>();
	death_zone->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - 1, v_window_w, v_window_h + 8 ) );
	death_zone->collision_layer = cl_deathzone;

	paddle = add_entity<e_paddle>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );

	engine->window->set_mouse_mode( mouse_mode::locked );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			if( paddle )
			{
				paddle->pos.x += evt->mouse.delta.x;
				paddle->pos.x = w_clamp( paddle->pos.x, 0.0f, v_window_w );
			}
			return true;
		}
	}

	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::key_n )
		{
			//add_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.625f );
			//ball->add_component<ec_emitter>()->init( "fireball_trail" )->set_life_timer( 1000 );
		}
	}

	return false;
}

void layer_gameplay::update()
{
	w_layer::update();

	timer_brick_move->update();

	int elapsed_count = timer_brick_move->get_elapsed_count();

	if( elapsed_count > 0 )
	{
		if( new_row_spawn_countdown == 0 )
		{
			spawn_new_brick_row( this );
			new_row_spawn_countdown = 25;
		}
		new_row_spawn_countdown--;

		for( auto& ent : entities )
		{
			if( ent->collision_layer & cl_brick )
			{
				ent->set_pos( { ent->pos.x, ent->pos.y + 1.0f } );
			}
		}
	}
}
