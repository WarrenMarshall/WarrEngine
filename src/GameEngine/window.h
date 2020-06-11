#pragma once

/*
	the actual OS window on the screen.
*/

struct w_window
{
	bool is_fullscreen = false;
	w_rect save_window_rect;
	std::string base_title;

	GLFWwindow* window = nullptr;
	GLFWmonitor* primary_monitor = nullptr;
	const GLFWvidmode* vidmode = nullptr;
	struct
	{
		int x, y, w, h;
	} save_windowed_state;

	bool init( const std::string& title );
	void deinit();
	void toggle_fullscreen();
	void set_title( const std::string& title );

	w_rect viewport_pos_sz;
};
