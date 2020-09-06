#pragma once

// ----------------------------------------------------------------------------

struct endless_breakout_game : w_game
{
	endless_breakout_game();

	a_music* mus_game = nullptr;
	a_sound* snd_game_over = nullptr;

	int score = 0;

	std::unique_ptr<w_timer> timer_brick_move;
	int brick_move_speed = 250;

	std::vector<a_subtexture*> brick_textures;
	a_subtexture* st_powerup_fireball = nullptr;
	a_subtexture* st_powerup_multiball = nullptr;

	virtual void init() final;
	virtual void new_game() final;
	virtual void update() final;
	void add_score( int points );
};
