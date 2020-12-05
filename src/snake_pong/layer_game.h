#pragma once

struct layer_game : w_layer
{
	layer_game();

	virtual void push() override;
	virtual void pop() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void becoming_top_layer() override;
};
