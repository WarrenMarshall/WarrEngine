#pragma once

// ----------------------------------------------------------------------------

struct hello_world_app : w_game
{
	hello_world_app( const char* name );

	void init() override;
	void new_game() override;
	void update() override;
};
