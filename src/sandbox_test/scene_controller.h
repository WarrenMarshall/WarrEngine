
using namespace war;

struct scene_controller_callback final : UI_Callback
{
	scene_controller_callback();

	virtual UI_Control_Data* get_data( hash tag ) override;

	UI_Control_Data progress_data_l2 = UI_Control_Data( e_ui_control_type::progress );
	UI_Control_Data progress_data_r2 = UI_Control_Data( e_ui_control_type::progress );
};

// ----------------------------------------------------------------------------

struct scene_controller final : scene
{
	bool use_controller_dead_zone = true;

	Tween fader;

	scene_controller();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
