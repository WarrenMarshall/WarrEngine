
#include "app_header.h"

int main( int argc, char* argv [] )
{
	auto game = endless_adventure_game();
	if( !w_engine::init_game_engine( "endless_adventure", argc, argv, &game ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 0;
}
