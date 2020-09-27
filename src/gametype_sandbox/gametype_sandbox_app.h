#pragma once

// ----------------------------------------------------------------------------

struct gametype_sandbox_app : w_game
{
	gametype_sandbox_app( std::string_view name );

	a_sound* snd_plat_jump = nullptr;
	a_sound* snd_plat_coin = nullptr;
	a_sound* snd_plat_drop_down = nullptr;

	a_sound* snd_pong_ball_hit_world = nullptr;
	a_sound* snd_pong_ball_hit_ball = nullptr;
	a_sound* snd_pong_ball_hit_paddle = nullptr;

	virtual void init() override;
	virtual void new_game() override;
	virtual void return_to_main_menu() override;
};
