
#pragma once

struct layer_esc_menu : w_layer
{
	virtual void push() override;
	virtual void pop() override;
	virtual void draw() override;
};
