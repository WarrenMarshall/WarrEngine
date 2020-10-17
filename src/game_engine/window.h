#pragma once

/*
	the actual OS window on the screen.
*/

struct w_window
{
	w_color v_window_clear_color = w_color::dark_grey;
	w_color window_clear_color = w_color::light_grey;
	bool is_fullscreen = false;
	w_rect save_window_rect = w_rect( 0, 0, 0, 0 );
	e_mouse_mode mouse_mode = mouse_mode::normal;

	GLFWwindow* window = nullptr;
	GLFWmonitor* primary_monitor = nullptr;
	const GLFWvidmode* vidmode = nullptr;
	struct
	{
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	} save_windowed_state;

	_NODISCARD w_rect compute_max_window_size_for_desktop();
	bool init();
	void deinit();
	void toggle_fullscreen();
	void set_title( const std::string_view title );
	void set_mouse_mode( e_mouse_mode mouse_mode );
	void refresh_mouse_mode();

	w_rect viewport_pos_sz;
};
