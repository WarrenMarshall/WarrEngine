
#include "app_header.h"

int main( int argc, char* argv [] )
{
	base_game = std::make_unique<template_platformer>( ADD_QUOTES( APP_NAME ) );

	if( !w_engine::init_game_engine( argc, argv ) )
	{
		return -1;
	}

	w_engine::exec_main_loop();

	w_engine::deinit_game_engine();

	return 1;
}
