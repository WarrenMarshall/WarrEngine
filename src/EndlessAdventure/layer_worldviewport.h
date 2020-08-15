#pragma once

struct layer_worldviewport : w_layer
{
	void push() override;
	void draw() override;
};
