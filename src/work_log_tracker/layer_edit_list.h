#pragma once

struct layer_edit_list : w_layer
{
	virtual void push() override;
	virtual void draw() override;
};
