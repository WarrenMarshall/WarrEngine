
namespace war
{

// the actual OS window on the screen.

struct OS_Window final
{
	// clear color for the entire window
	Color window_clear_color = Color::light_grey;

	// clear color for the opengl viewport
	Color viewport_clear_color = Color::dark_grey;

	bool is_fullscreen = false;
	//e_mouse_mode mouse_mode = e_mouse_mode::os;
	std::stack<e_mouse_mode> mouse_mode_stack;

	GLFWwindow* glfw_window = nullptr;
	GLFWmonitor* primary_monitor = nullptr;
	const GLFWvidmode* vidmode = nullptr;
	struct
	{
		int32_t x, y, w, h;
	} save_windowed_state;

	[[nodiscard]] Rect compute_max_window_size_for_desktop();
	bool init();
	void deinit();
	void toggle_fullscreen();
	void set_title( std::string_view title );
	void push_mouse_mode( e_mouse_mode mouse_mode );
	void pop_mouse_mode();
	void refresh_mouse_mode();

	Rect viewport_pos_sz;
};

}
