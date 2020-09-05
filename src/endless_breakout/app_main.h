#pragma once

// ----------------------------------------------------------------------------

struct endless_breakout_game final : w_game
{
	endless_breakout_game();

	a_sound* sfx_paddle_impact = nullptr;
	a_sound* sfx_ball_impact = nullptr;
	a_sound* sfx_deathzone_impact = nullptr;
	a_sound* sfx_wall_impact = nullptr;
	a_sound* sfx_brick_impact = nullptr;

	a_music* mus_game = nullptr;

	std::vector<a_subtexture*> brick_textures;
	a_subtexture* st_powerup_fireball = nullptr;
	a_subtexture* st_powerup_multiball = nullptr;

	void init() final;
	void new_game() final;
};
