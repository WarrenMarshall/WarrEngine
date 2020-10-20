#pragma once

struct layer_main_menu : w_layer
{
	layer_main_menu();

	virtual void push() override;
	virtual void pop() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void becoming_top_layer() override;
};
