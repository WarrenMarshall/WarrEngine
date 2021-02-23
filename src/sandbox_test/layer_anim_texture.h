#pragma once

struct layer_anim_texture : w_layer
{
	a_anim_texture* animtex_01 = nullptr;
	a_anim_texture* animtex_coin_01 = nullptr;

	layer_anim_texture();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
