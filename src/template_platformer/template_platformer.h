#pragma once

// ----------------------------------------------------------------------------

struct template_platformer : w_game
{
	template_platformer( std::string_view name );

	a_sound* snd_jump = nullptr;
	a_sound* snd_coin = nullptr;
	a_sound* snd_drop_down = nullptr;

	a_sound* music_main_menu = nullptr;

	std::vector<a_subtexture*> tile_set_subtex;

	w_entity* player = nullptr;
	w_camera* player_camera = nullptr;

	virtual void init() override;
	virtual void reset_layer_stack_to_main_menu() override;
	virtual std::string get_game_name() override;

	void load_level( std::string_view level_filename );
	void spawn_coins();
};
