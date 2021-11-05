
using namespace war;

// ----------------------------------------------------------------------------

struct Scene_Wrapped_Text_UI_Callback final : UI_Callback
{
	UI_Control_Data radio_alignment_data;

	virtual UI_Control_Data* get_data( hash tag ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Wrapped_Text final : Scene
{
	Scene_Wrapped_Text_UI_Callback uic;

	std::string big_text_block;

	std::vector<std::string> wrapped_lines_01;
	Rect rc_big_text_block_01 = {};

	std::vector<std::string> wrapped_lines_02;
	Rect rc_big_text_block_02 = {};
	f32 text_pct = 0.0f;

	Scene_Wrapped_Text();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input( const Input_Event* evt ) override;

};
