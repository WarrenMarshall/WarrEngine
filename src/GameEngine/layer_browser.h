#pragma once

struct layer_browser : w_layer
{
	a_subtexture* selector_bracket = nullptr;
	w_vec2 hover_tile = w_vec2( 0, 0 );

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;

	void on_listener_event_received( e_event_id event, void* object );
	int tile_from_screen_pos( float xpos, float ypos );
};
