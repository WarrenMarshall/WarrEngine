
namespace war
{

struct Scene_Msg_Box_UI_Callback : UI_Callback
{
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Msg_Box : Scene
{
	Scene_Msg_Box_UI_Callback uic;

	Scene_Msg_Box();

	std::vector<std::string> msg;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
};

}
