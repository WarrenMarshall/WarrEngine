#pragma once

struct layer_background : w_layer
{
	virtual void draw();
	virtual e_opaque get_opaque_flags();
};
