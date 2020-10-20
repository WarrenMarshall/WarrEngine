#pragma once

struct twinstick_layer : w_layer
{
	std::unique_ptr<w_twinstick_physics> twinstick_physics = nullptr;

	e_twinstick_player* player = nullptr;
	w_camera* player_camera = nullptr;
	a_sound* music = nullptr;

	twinstick_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual w_camera* get_camera() override;
};
