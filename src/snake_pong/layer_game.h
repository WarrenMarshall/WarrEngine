#pragma once

struct layer_game : w_layer
{
	layer_game();

	virtual void push() override;
	virtual void pop() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void becoming_top_layer() override;
	virtual w_camera* get_camera() override;
	virtual void update() override;

	virtual bool iir_on_pressed( const w_input_event* evt ) override;
	void spawn_ball();
};
