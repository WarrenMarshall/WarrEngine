
namespace war
{

struct scene_msg_box_ui_callback final : ui_callback
{
	virtual bool on_input_pressed( const input_event* evt ) override;
};

// ----------------------------------------------------------------------------

struct scene_msg_box final : scene
{
	scene_msg_box();

	std::vector<std::string> msg;
	e_mouse_mode save_mouse_mode;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
};

}
