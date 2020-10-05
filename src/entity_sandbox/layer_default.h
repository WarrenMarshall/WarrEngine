#pragma once

struct layer_default : w_layer
{
	layer_default();

	w_entity* test_entity = nullptr;
	std::unique_ptr<w_tween> test_tween = nullptr;

	virtual void push() override;
	virtual void draw() override;
	virtual void update() override;

	virtual bool handle_input_event( const w_input_event* evt );
};
