
#include "app_header.h"

int main( int argc, char* argv [] )
{
	game = std::make_unique<gametype_sandbox_app>( std::string( ADD_QUOTES( APP_NAME ) ) );

	if( !w_engine::init_game_engine( argc, argv ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 0;
}