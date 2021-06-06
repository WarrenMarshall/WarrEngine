
namespace war
{

// ----------------------------------------------------------------------------

struct scene_esc_menu_ui_callback final : ui_callback
{
	ui_control_data data_fullscreen;

	virtual ui_control_data* get_data( hash tag ) override;

	virtual bool on_input_pressed( const input_event* evt ) override;
};

// ----------------------------------------------------------------------------

struct scene_esc_menu final : scene
{
	scene_esc_menu();

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
};

}
