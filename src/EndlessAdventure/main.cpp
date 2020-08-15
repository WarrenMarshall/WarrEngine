
#include "app_header.h"

int main( int argc, char* argv [] )
{
	endless_adventure_game custom_game;
	if( !w_engine::init_game_engine( "endless_adventure", argc, argv, &custom_game ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 0;
}
