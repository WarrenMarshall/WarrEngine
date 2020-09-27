
#include "app_header.h"

gametype_sandbox_app::gametype_sandbox_app( std::string_view name )
	: w_game( name )
{
}

void gametype_sandbox_app::init()
{
	w_game::init();

	snd_plat_jump = engine->get_asset<a_sound>( "snd_plat_jump" );
	snd_plat_coin = engine->get_asset<a_sound>( "snd_plat_coin" );
	snd_plat_drop_down = engine->get_asset<a_sound>( "snd_plat_drop_down" );

	snd_pong_ball_hit_world = engine->get_asset<a_sound>( "snd_pong_ball_hit_world" );
	snd_pong_ball_hit_ball = engine->get_asset<a_sound>( "snd_pong_ball_hit_ball" );
	snd_pong_ball_hit_paddle = engine->get_asset<a_sound>( "snd_pong_ball_hit_paddle" );
}

void gametype_sandbox_app::new_game()
{
	w_game::new_game();

	return_to_main_menu();
}

void gametype_sandbox_app::return_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();
}
