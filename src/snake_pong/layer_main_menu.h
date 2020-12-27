#pragma once

struct layer_main_menu : w_layer
{
	layer_main_menu();

	virtual void draw_ui() override;
	virtual void becoming_top_layer() override;
	virtual void push() override;
	virtual void new_game() override;

	virtual bool iir_on_pressed( const w_input_event* evt ) override;
};
