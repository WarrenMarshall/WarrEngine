
using namespace war;

// ----------------------------------------------------------------------------

struct scene_wrapped_text_ui_callback final : ui_callback
{
	ui_control_data radio_alignment_data;

	virtual ui_control_data* get_data( hash tag ) override;
};

// ----------------------------------------------------------------------------

struct scene_wrapped_text final : scene
{
	scene_wrapped_text();

	std::string big_text_block;

	std::vector<std::string> wrapped_lines_01;
	rect rc_big_text_block_01 = {};

	std::vector<std::string> wrapped_lines_02;
	rect rc_big_text_block_02 = {};

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;


};
