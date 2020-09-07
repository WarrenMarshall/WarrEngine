#pragma once

struct layer_firetoplay : w_layer
{
	std::unique_ptr<w_tween> scale_tween = nullptr;

	virtual void push() final;
	virtual void pop() final;
	virtual void draw() final;
	virtual void update() final;
	virtual bool handle_input_event( const w_input_event* evt ) final;
};

