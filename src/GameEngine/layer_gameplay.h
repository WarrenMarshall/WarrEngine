#pragma once

struct layer_gameplay : w_layer
{
	virtual void push();
	virtual void pop();
	virtual void draw();
	void on_listener_event_received( e_event_id event, void* object );
};
