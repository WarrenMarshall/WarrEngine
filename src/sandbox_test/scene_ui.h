
using namespace war;

struct scene_ui_callback final : ui_callback
{
	scene_ui_callback();

	virtual ui_control_data* get_data( hash tag ) override;
	virtual size_t get_item_count( hash tag ) override;
	virtual std::string_view get_item_for_idx( hash tag, int idx ) override;
	virtual void on_value_changed( hash tag ) override;

	ui_control_data checkbox_data;
	ui_progress_control_data progress_data;
	ui_slider_control_data slider_01_data;
	ui_slider_control_data slider_02_data;
	ui_text_control_data text_data;
	ui_control_data radio_data;
	ui_control_data list_data;
	ui_dropdown_control_data dropdown_data;

	std::vector<std::string> list_items;
	std::vector<std::string> dropdown_items;
};

// ----------------------------------------------------------------------------

struct scene_ui : scene
{
	scene_ui();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
