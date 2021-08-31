
using namespace war;

struct Scene_Controller_UI_Callback : UI_Callback
{
	Scene_Controller_UI_Callback();

	virtual UI_Control_Data* get_data( hash tag ) override;

	UI_Control_Data progress_data_l2 = UI_Control_Data( e_ui_control_type::progress );
	UI_Control_Data progress_data_r2 = UI_Control_Data( e_ui_control_type::progress );
};

// ----------------------------------------------------------------------------

struct Scene_Controller : Scene
{
	bool use_controller_dead_zone = true;

	Tween fader;

	Scene_Controller();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
