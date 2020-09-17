#pragma once

// ----------------------------------------------------------------------------

struct hello_world_app : w_game
{
	hello_world_app( std::string_view name );

	void init() override;
	void new_game() override;
	void update() override;
};
