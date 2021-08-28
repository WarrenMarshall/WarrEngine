
using namespace war;

// ----------------------------------------------------------------------------

struct scene_wrapped_text_ui_callback final : UI_Callback
{
	UI_Control_Data radio_alignment_data;

	virtual UI_Control_Data* get_data( hash tag ) override;
};

// ----------------------------------------------------------------------------

struct scene_wrapped_text final : scene
{
	scene_wrapped_text();

	std::string big_text_block;

	std::vector<std::string> wrapped_lines_01;
	Rect rc_big_text_block_01 = {};

	std::vector<std::string> wrapped_lines_02;
	Rect rc_big_text_block_02 = {};

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;


};
