
#include "app_header.h"

constexpr int multiball_spawn_interval = 50;
constexpr int fireball_spawn_interval = 35;

void spawn_new_brick_row( layer_gameplay* layer )
{
	static int spawned_brick_counter = 0;
	static int brick_color_idx = -1;

	brick_color_idx = ( brick_color_idx + 1 ) % static_cast<int>( GAME->brick_textures.size() );
	std::string subtex_name = GAME->brick_textures[ brick_color_idx ]->name;

	int xpos = 24 + w_random::geti_range( 0, 64 );
	for( int x = 0 ; x < 5 ; ++x )
	{
		if( w_random::geti_range( 0, 3 ) != 0 )
		{
			spawned_brick_counter++;

			if( !(spawned_brick_counter % multiball_spawn_interval) )
			{
				layer->add_entity<e_brick_multiball>( { xpos, -16 }, 0.0f, 0.725f );
			}
			else if( !( spawned_brick_counter % fireball_spawn_interval ) )
			{
				layer->add_entity<e_brick_fireball>( { xpos, -16 }, 0.0f, 0.725f );
			}
			else
			{
				auto brick = layer->add_entity<e_brick>( { xpos, -16 }, 0.0f, 0.75f );
				brick->add_component<ec_sprite>()->init( subtex_name );
			}
		}
		xpos += 48;
	}
}

void layer_gameplay::push()
{
	ball = add_entity<e_ball>( { v_window_hw, 32.0f }, 0.0f, 1.0f );
	//ball->debug_draw_collision = true;

	auto walls = add_entity<w_entity>();

	// left
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, 0.0f, 1.0f, v_window_h ) );
	// right
	walls->add_component<ec_collider>()->init_as_box( w_rect( v_window_w - 1.0f, 0.0f, v_window_w, v_window_h ) );
	// top (extra thicc to prevent multiballs from spawning outside the world)
	walls->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, -64.0f, v_window_w, 66.0f ) );

	walls->collision_layer = cl_wall;

	auto death_zone = add_entity<w_entity>();
	death_zone->add_component<ec_collider>()->init_as_box( w_rect( 0.0f, v_window_h - 1, v_window_w, v_window_h + 8 ) );
	death_zone->collision_layer = cl_deathzone;

	paddle = add_entity<e_paddle>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );
	//paddle->debug_draw_collision = true;

	engine->window->set_mouse_mode( mouse_mode::locked );
}

void layer_gameplay::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	GAME->mus_game->play();
}

void layer_gameplay::getting_covered()
{
	w_layer::getting_covered();

	GAME->mus_game->stop();
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
		}
	}

	return false;
}

void layer_gameplay::update()
{
	if( LAYER != this )
	{
		return;
	}

	w_layer::update();

	int elapsed_count = GAME->timer_brick_move->get_elapsed_count();

	if( elapsed_count > 0 )
	{
		if( new_row_spawn_countdown == 0 )
		{
			spawn_new_brick_row( this );
			new_row_spawn_countdown = 24;
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

void layer_gameplay::draw()
{
	w_layer::draw();

	// score / high_score

	RENDER
		->begin()
		->push_depth_nudge( 500 )
		->push_align( align::centered )
		->push_rgb( w_color::white )
		->push_alpha( 0.15f )
		->push_scale( 6.0f )
		->draw_string( engine->pixel_font, f_commas( static_cast<float>( GAME->display_score ) ), w_rect( v_window_hw, v_window_h - 100 ) );

	if( GAME->high_score > 0 )
	{
		RENDER
			->push_rgb( w_color::grey )
			->push_alpha( 0.15f )
			->push_scale( 2.0f )
			->draw_string( engine->pixel_font, f_commas( static_cast<float>( GAME->high_score ) ), w_rect( v_window_hw, v_window_h - 60 ) );
	}

	RENDER
		->end();

	// death zone

	RENDER
		->begin()
		->push_depth_nudge()
		->push_rgba( w_color( 1.0, 0.5f, 0.0f, 0.5 ) )
		->draw_filled_rectangle( w_rect( 0.0f, v_window_h - 5, v_window_w, v_window_h + 8 ) )
		->push_depth_nudge()
		->push_rgba( w_color( 1.0f, 0.0f, 0.0f, w_random::getf_range( 0.5f, 1.0f ) ) )
		->draw_line( { 0.0f, v_window_h - 5 }, { v_window_w, v_window_h - 5 } )
		->draw_line( { 0.0f, v_window_h - 4 }, { v_window_w, v_window_h - 4 } )
		->end();

}