#pragma once

struct twinstick_layer : w_layer
{
	std::unique_ptr<w_twinstick_physics> twinstick_physics = nullptr;

	e_twinstick_player* player = nullptr;

	twinstick_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;

	virtual bool handle_input_event( const w_input_event* evt ) override;
};
