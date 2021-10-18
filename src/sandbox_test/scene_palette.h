
using namespace war;

struct Scene_Palette_Callback final : UI_Callback
{
	Scene_Palette_Callback();

	std::vector<std::string> palette_items;
	std::vector<std::string> lut_items;

	UI_Control_Data palette_data = UI_Control_Data( e_ui_control_type::dropdown );
	UI_Control_Data lut_data = UI_Control_Data( e_ui_control_type::dropdown );

	virtual UI_Control_Data* get_data( hash tag ) override;
	virtual size_t get_item_count( hash tag ) override;
	virtual std::string_view get_item_for_idx( hash tag, int idx ) override;
	virtual void on_value_changed( hash tag ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Palette final : Scene
{
	Scene_Palette_Callback uic;

	Scene_Palette();

	virtual void draw() override;
	virtual void draw_ui() override;
};
