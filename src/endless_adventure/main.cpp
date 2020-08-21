
#include "app_header.h"

int main( int argc, char* argv [] )
{
	auto game = std::make_unique<endless_adventure_game>();

	// warren - w/h are ignored
	if( !w_engine::init_game_engine( "endless_adventure", argc, argv, game.get() ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 0;
}
