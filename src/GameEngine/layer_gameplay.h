#pragma once

struct layer_gameplay : w_layer
{
	std::unique_ptr<w_image> img_atlas = nullptr;
	std::unique_ptr<w_image> img_gradient = nullptr;

	std::unique_ptr<w_image> img_grass = nullptr;
	std::unique_ptr<w_image> img_town = nullptr;
	std::unique_ptr<w_image> img_player = nullptr;

	virtual void push();
	virtual void pop();
	virtual void update();
	virtual void draw();
	void on_listener_event_received( e_event_id event, void* object );
};
