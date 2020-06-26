#pragma once

struct layer_editor : w_layer
{
	i_transform pingpong_xform;
	std::unique_ptr<w_tween> tween_pingpong;
	i_transform rotate_xform;
	std::unique_ptr<w_tween> tween_rotate;

	virtual void push();
	virtual void pop();
	virtual void update();
	virtual void draw();
	void on_listener_event_received( e_event_id event, void* object );
};
