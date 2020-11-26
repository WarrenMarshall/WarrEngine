#pragma once

struct twinstick_layer : w_layer
{
	std::unique_ptr<w_twinstick_physics_responder> twinstick_physics = nullptr;

	e_twinstick_player* player = nullptr;
	w_vec2 trace_hit_location = w_vec2::zero;
	w_camera* player_camera = nullptr;
	a_sound* music = nullptr;

	twinstick_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual w_camera* get_camera() override;

	virtual bool event_input_motion( const w_input_event* evt ) override;
	virtual bool event_input_pressed( const w_input_event* evt ) override;
	virtual bool event_input_held( const w_input_event* evt ) override;
	virtual bool event_input_released( const w_input_event* evt ) override;
};
