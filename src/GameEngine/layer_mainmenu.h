#pragma once

struct layer_mainmenu : w_layer
{
	a_gradient* grad_teal_blue = nullptr;
	a_texture* germ = nullptr;
	a_texture* logo = nullptr;
	a_music* mus_mainmenu = nullptr;

	virtual void push();
	virtual void becoming_top_layer();
	virtual void getting_covered();
	virtual void draw();
};
