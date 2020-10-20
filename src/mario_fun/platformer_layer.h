#pragma once

struct platformer_layer : w_layer
{
	std::unique_ptr<w_platformer_physics> plat_physics = nullptr;
	a_gradient* sky_gradient = nullptr;
	a_sound* music_gameplay = nullptr;

	platformer_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt ) override;
	virtual w_camera* get_camera() override;
};
