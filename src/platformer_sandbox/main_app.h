#pragma once

// ----------------------------------------------------------------------------

struct main_app final : w_game
{
	main_app();

	void init() final;
	void new_game() final;
	void update() final;
};
