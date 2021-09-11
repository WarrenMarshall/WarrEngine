
using namespace war;

struct Scene_UI_Callback final : UI_Callback
{
	std::vector<std::string> list_items;
	std::vector<std::string> dropdown_items;

	// ----------------------------------------------------------------------------

	Scene_UI_Callback();

	virtual UI_Control_Data* get_data( hash tag ) override;
	virtual size_t get_item_count( hash tag ) override;
	virtual std::string_view get_item_for_idx( hash tag, int idx ) override;
	virtual void on_value_changed( hash tag ) override;

	UI_Control_Data checkbox_data = UI_Control_Data( e_ui_control_type::check );
	UI_Control_Data progress_data = UI_Control_Data( e_ui_control_type::progress );
	UI_Control_Data slider_01_data = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data slider_02_data = UI_Control_Data( e_ui_control_type::slider );
	UI_Control_Data text_data = UI_Control_Data( e_ui_control_type::text );
	UI_Control_Data radio_data = UI_Control_Data( e_ui_control_type::radio );
	UI_Control_Data list_data = UI_Control_Data( e_ui_control_type::list );
	UI_Control_Data dropdown_data = UI_Control_Data( e_ui_control_type::dropdown );
};

// ----------------------------------------------------------------------------

struct Scene_UI final : Scene
{
	Scene_UI_Callback uic;

	// ----------------------------------------------------------------------------

	Scene_UI();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
