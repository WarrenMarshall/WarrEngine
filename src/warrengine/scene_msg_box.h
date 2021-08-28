
namespace war
{

struct Scene_Msg_Box_UI_Callback final : UI_Callback
{
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Msg_Box final : scene
{
	Scene_Msg_Box();

	std::vector<std::string> msg;
	e_mouse_mode save_mouse_mode;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
};

}
