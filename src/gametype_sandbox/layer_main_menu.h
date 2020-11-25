#pragma once

struct layer_main_menu : w_layer
{
	layer_main_menu();

	virtual void push() override;
	virtual void draw() override;
	virtual void becoming_top_layer() override;
	virtual bool event_input_pressed( const w_input_event* evt ) override;
};
