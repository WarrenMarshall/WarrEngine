
using namespace war;

struct scene_gameplay : scene
{
	e_player* player = nullptr;

	ui_progress_control_data ship_power_progress;
	timer ship_power_timer;

	scene_gameplay();

	virtual void draw_ui() override;

	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
};
