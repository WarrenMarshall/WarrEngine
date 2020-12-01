#pragma once

// ----------------------------------------------------------------------------

struct template_platformer : w_game
{
	template_platformer( const char* name );

	std::unique_ptr<w_platformer_physic_responder> physics_responder = nullptr;

	a_sound* snd_jump = nullptr;
	a_sound* snd_coin = nullptr;
	a_sound* snd_drop_down = nullptr;

	a_sound* music_main_menu = nullptr;

	std::unique_ptr<w_player_input_controller> input_controller = nullptr;

	std::vector<a_subtexture*> tile_set_subtex;

	w_entity* player = nullptr;
	w_camera* player_camera = nullptr;

	virtual void init() override;
	virtual void reset_layer_stack_to_main_menu() override;

	void load_level( std::string_view level_filename );
	void spawn_coins();
};
