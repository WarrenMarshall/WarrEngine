#pragma once

// ----------------------------------------------------------------------------

struct endless_breakout_game final : w_game
{
	endless_breakout_game();

	void init() final;
	void new_game() final;
	void update() final;
};
