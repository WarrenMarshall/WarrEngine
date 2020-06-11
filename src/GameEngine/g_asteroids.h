#pragma once

struct g_asteroids : w_game
{
	a_font* font = nullptr;
	e_player* player = nullptr;
	a_sound* snd_fireball_spawn = nullptr;

	std::unique_ptr<w_timer> fireball_spawn_timer = nullptr;

	int fireballs_spawned = 0;
	int player_hits = 0;
	int fireball_spawn_rate = 2000;

	g_asteroids();

	virtual void new_game();
	virtual void update();
	virtual void update_fts();
};
