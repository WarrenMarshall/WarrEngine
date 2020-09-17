#pragma once

// ----------------------------------------------------------------------------

struct hello_world_app final : w_game
{
	hello_world_app( std::string_view name );

	void init() final;
	void new_game() final;
	void update() final;
};
