#pragma once

struct layer_firetoplay : w_layer
{
	virtual void push() final;
	virtual void pop() final;
	virtual void draw() final;
	virtual bool handle_input_event( const w_input_event* evt ) final;
};

