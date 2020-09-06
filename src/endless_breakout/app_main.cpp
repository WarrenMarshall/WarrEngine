
#include "app_header.h"

endless_breakout_game::endless_breakout_game()
{
}

void endless_breakout_game::init()
{
	mus_game = engine->get_asset<a_music>( "music_game" );
	snd_game_over = engine->get_asset<a_sound>( "game_over" );

	st_powerup_fireball = engine->get_asset<a_subtexture>( "sub_brick_red" );
	st_powerup_multiball = engine->get_asset<a_subtexture>( "sub_brick_yellow" );

	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_grey" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_blue" ) );
	brick_textures.push_back( engine->get_asset<a_subtexture>( "sub_brick_green" ) );
}

void endless_breakout_game::new_game()
{
	w_game::new_game();

	engine->layer_mgr->layer_stack.clear();

	engine->layer_mgr->push( std::make_unique<layer_background>() );
	engine->layer_mgr->push( std::make_unique<layer_gameplay>() );
	engine->layer_mgr->push( std::make_unique<layer_firetoplay>() );

	score = 0;
	brick_move_speed = 250;
	timer_brick_move = std::make_unique<w_timer>( brick_move_speed );
}

void endless_breakout_game::add_score( int points )
{
	score += points;

	if( ( score % 150 ) == 0 )
	{
		brick_move_speed -= 5;
		brick_move_speed = w_max( brick_move_speed, 10 );

		timer_brick_move->interval_ms = static_cast<float>( brick_move_speed );
		timer_brick_move->reset();
	}
}

void endless_breakout_game::update()
{
	w_game::update();

	timer_brick_move->update();
}
