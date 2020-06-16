#pragma once

struct g_custom_game : w_game
{
	a_font* font = nullptr;
	e_player* player = nullptr;

	g_custom_game();

	virtual void new_game();
	virtual void update();
	virtual void update_fts();
};
