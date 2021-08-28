
using namespace war;

struct scene_ui_callback final : UI_Callback
{
	scene_ui_callback();

	virtual UI_Control_Data* get_data( hash tag ) override;
	virtual size_t get_item_count( hash tag ) override;
	virtual std::string_view get_item_for_idx( hash tag, int idx ) override;
	virtual void on_value_changed( hash tag ) override;

	UI_Control_Data checkbox_data;
	UI_Progress_Control_Data progress_data;
	UI_Slider_Control_Data slider_01_data;
	UI_Slider_Control_Data slider_02_data;
	UI_Text_Control_Data text_data;
	UI_Control_Data radio_data;
	UI_Control_Data list_data;
	UI_Dropdown_Control_Data dropdown_data;

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
