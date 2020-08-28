#pragma once

struct w_game
{
	w_game() = default;
	virtual ~w_game() = default;

	virtual void init() {}
	virtual void new_game();
	virtual void update();
};
