#pragma once

struct layer_default_data_provider : w_imgui_data_provider
{
	virtual int get_subtexture_idx( w_imgui_control* control ) override;

	bool the_checkbox_value_01 = false;
};

// ----------------------------------------------------------------------------

struct layer_default : w_layer
{
	a_gradient* gradient = nullptr;
	float angle = 0.0f;

	layer_default_data_provider ui_data_provider;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;
};
