
#include "app_header.h"

mario_fun_app::mario_fun_app( std::string_view name )
	: w_game( name )
{
}

void mario_fun_app::init()
{
	w_game::init();

	snd_plat_jump = engine->get_asset<a_sound>( "snd_plat_jump" );
	snd_plat_coin = engine->get_asset<a_sound>( "snd_plat_coin" );
	snd_plat_drop_down = engine->get_asset<a_sound>( "snd_plat_drop_down" );
}

void mario_fun_app::new_game()
{
	w_game::new_game();

	return_to_main_menu();
}

void mario_fun_app::return_to_main_menu()
{
	engine->layer_mgr->clear_stack();
	engine->layer_mgr->push<layer_main_menu>();
}
