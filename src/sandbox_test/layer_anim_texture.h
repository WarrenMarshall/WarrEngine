#pragma once

// ----------------------------------------------------------------------------

struct layer_anim_texture_callback : w_imgui_callback
{
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
	virtual void on_motion( const w_imgui_control& control, const w_imgui_result& result ) override;
};

// ----------------------------------------------------------------------------

struct layer_anim_texture : w_layer
{
	layer_anim_texture_callback imgui_callback;

	a_anim_texture* animtex_01 = nullptr;
	a_anim_texture* animtex_coin_01 = nullptr;

	layer_anim_texture();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
