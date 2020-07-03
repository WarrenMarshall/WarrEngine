#pragma once

/*
	the actual OS window on the screen.
*/

struct w_window
{
	bool is_fullscreen = false;
	w_rect save_window_rect = w_rect( 0, 0, 0, 0 );
	std::string base_title;

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

	bool init( const std::string& title );
	void deinit();
	void toggle_fullscreen();
	void set_title( const std::string& title );
	void set_mouse_mode( e_mouse_mode mode );

	w_rect viewport_pos_sz;
};
