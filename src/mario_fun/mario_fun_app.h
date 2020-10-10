#pragma once

// ----------------------------------------------------------------------------

struct mario_fun_app : w_game
{
	mario_fun_app( std::string_view name );

	a_sound* snd_plat_jump = nullptr;
	a_sound* snd_plat_coin = nullptr;
	a_sound* snd_plat_drop_down = nullptr;

	virtual void init() override;
	virtual void new_game() override;
	virtual void return_to_main_menu() override;
};
