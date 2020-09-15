#pragma once

struct w_game
{
	w_game( std::string_view name );
	virtual ~w_game() = default;

	virtual void init();
	virtual void new_game();
	virtual void update();

	std::string name = "";
};
