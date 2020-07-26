#pragma once

struct layer_editor : w_layer
{
	w_rect tile_display_area;
	a_anim_texture* germ = nullptr;
	a_gradient* grad = nullptr;

	bool is_painting = false;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
	bool handle_input_event( const w_input_event* evt ) override;

	void set_current_tile_from_mouse_pos( float xpos, float ypos );
	void paint_current_tile();
	void set_current_tile_idx_from_current_tile();
};
