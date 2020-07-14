#pragma once

struct layer_editor : w_layer
{
	a_subtexture* selector_bracket = nullptr;
	a_9slice_def* panel_slice_def = nullptr;

	w_vec2 current_tile = w_vec2( 0.0f, 0.0f );
	bool is_painting = false;
	int current_tile_idx = 0;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
	void on_listener_event_received( e_event_id event, void* object ) override;

	void set_current_tile_from_mouse_pos( float xpos, float ypos );
	void paint_current_tile();
	void set_current_tile_idx_from_current_tile();
};
