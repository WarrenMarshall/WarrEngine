
#include "app_header.h"

endless_breakout_game::endless_breakout_game()
{
}

void endless_breakout_game::init()
{
	mus_game = engine->get_asset<a_music>( "music_game" );
	snd_game_over = engine->get_asset<a_sound>( "game_over" );

	timer_display_score_update = std::make_unique<w_timer>( 20 );

	st_powerup_fireball = engine->get_asset<a_subtexture>( "brick_red" );
	st_powerup_multiball = engine->get_asset<a_subtexture>( "brick_yellow" );

	brick_textures.push_back( engine->get_asset<a_subtexture>( "brick_blue_01" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "brick_blue_02" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "brick_blue_03" ) );
}

void endless_breakout_game::new_game()
{
	w_game::new_game();

	engine->layer_mgr->layer_stack.clear();

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_gameplay>() );
	engine->layer_mgr->push( std::make_unique<layer_firetoplay>() );

	if( score > high_score )
	{
		high_score = score;
	}

	score = display_score = 0;
	brick_move_speed = 250;
	timer_brick_move = std::make_unique<w_timer>( brick_move_speed );
}

void endless_breakout_game::add_score( int points )
{
	score += points;

	if( ( score % 100 ) == 0 )
	{
		brick_move_speed -= 3;
		brick_move_speed = w_max( brick_move_speed, 50 );

		timer_brick_move->interval_ms = static_cast<float>( brick_move_speed );
		timer_brick_move->reset();
	}
}

void endless_breakout_game::update()
{
	w_game::update();

	timer_brick_move->update();
	timer_display_score_update->update();

	int score_ticks = timer_display_score_update->get_elapsed_count();
	if( display_score < score )
	{
		display_score += score_ticks;
		display_score = w_min( display_score, score );
	}
}
