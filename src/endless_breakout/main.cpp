
#include "app_header.h"

int main( int argc, char* argv [] )
{
	auto game = std::make_unique<endless_breakout_game>();

	if( !w_engine::init_game_engine( std::string( ADD_QUOTES( APP_NAME ) ), argc, argv, game.get() ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 0;
}
