
using namespace war;

struct scene_controller_callback final : ui_callback
{
	scene_controller_callback();

	virtual ui_control_data* get_data( hash tag ) override;

	ui_progress_control_data progress_data_l2;
	ui_progress_control_data progress_data_r2;
};

// ----------------------------------------------------------------------------

struct scene_controller final : scene
{
	bool use_controller_dead_zone = true;

	tween fader;

	scene_controller();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
};
