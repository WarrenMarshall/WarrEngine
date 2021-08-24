
namespace war
{

// the actual OS window on the screen.

struct os_window final
{
	// clear color for the entire window
	color window_clear_color = color::light_grey;

	// clear color for the opengl viewport
	color viewport_clear_color = color::dark_grey;

	bool is_fullscreen = false;
	e_mouse_mode mouse_mode = mouse_mode::os;

	GLFWwindow* glfw_window = nullptr;
	GLFWmonitor* primary_monitor = nullptr;
	const GLFWvidmode* vidmode = nullptr;
	struct
	{
		int x, y, w, h;
	} save_windowed_state;

	[[nodiscard]] rect compute_max_window_size_for_desktop();
	bool init();
	void deinit();
	void toggle_fullscreen();
	void set_title( std::string_view title );
	void set_mouse_mode( e_mouse_mode mouse_mode );
	void refresh_mouse_mode();

	rect viewport_pos_sz;
};

}
