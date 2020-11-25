
#include "app_header.h"

gametype_sandbox_app::gametype_sandbox_app( const char* name )
	: w_game( name )
{
}

void gametype_sandbox_app::init()
{
	w_game::init();

	snd_plat_jump = a_sound::find( "snd_plat_jump" );
	snd_plat_coin = a_sound::find( "snd_plat_coin" );
	snd_plat_drop_down = a_sound::find( "snd_plat_drop_down" );

	snd_pong_ball_hit_world = a_sound::find( "snd_pong_ball_hit_world" );
	snd_pong_ball_hit_ball = a_sound::find( "snd_pong_ball_hit_ball" );
	snd_pong_ball_hit_paddle = a_sound::find( "snd_pong_ball_hit_paddle" );
}

void gametype_sandbox_app::new_game()
{
	w_game::new_game();
}

void gametype_sandbox_app::reset_layer_stack_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();
}
