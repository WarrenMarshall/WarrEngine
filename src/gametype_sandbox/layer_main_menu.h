#pragma once

struct layer_main_menu : w_layer
{
	layer_main_menu();

	virtual void push() override;
	virtual void draw() override;
	virtual void becoming_top_layer() override;
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
