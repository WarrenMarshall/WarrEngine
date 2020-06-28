#pragma once

struct layer_editor : w_layer
{
	i_transform pingpong_xform;
	std::unique_ptr<w_tween> tween_pingpong;
	i_transform rotate_xform;
	std::unique_ptr<w_tween> tween_rotate;

	bool draw_selector_bracket = false;
	std::unique_ptr<a_image> selector_bracket = nullptr;
	a_cursor* mouse_cursor = nullptr;

	w_vec2 hover_tile = w_vec2( -1.0f, -1.0f );

	virtual void push();
	virtual void pop();
	virtual void update();
	virtual void draw();
	void on_listener_event_received( e_event_id event, void* object );

	int tile_from_screen_pos( float xpos, float ypos );
};
