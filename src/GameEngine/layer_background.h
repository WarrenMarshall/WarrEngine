#pragma once

struct layer_background : w_layer
{
	void draw() override;
	e_opaque get_opaque_flags() override;
};
