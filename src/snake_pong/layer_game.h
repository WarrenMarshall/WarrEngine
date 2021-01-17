#pragma once

struct layer_game : w_layer
{
	w_entity* prism = nullptr;
	bool power_flicker = false;

	layer_game();

	virtual void becoming_top_layer() override;
	virtual void getting_covered() override;
	virtual void update() override;

	virtual bool on_input_pressed( const w_input_event* evt ) override;
	void spawn_ball();

	virtual void new_game() override;
	virtual void draw() override;
};
