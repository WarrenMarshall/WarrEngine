#pragma once

struct layer_editor : w_layer
{
	a_subtexture* selector_bracket = nullptr;
	a_9slice_def* panel_slice_def = nullptr;

	c2AABB tile_display_area;
	w_rect rc_button_browse;

	bool is_painting = false;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
	void handle_input_event( const w_input_event* evt ) override;

	void set_current_tile_from_mouse_pos( float xpos, float ypos );
	void paint_current_tile();
	void set_current_tile_idx_from_current_tile();
};
