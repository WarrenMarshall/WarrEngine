
namespace war
{

// ----------------------------------------------------------------------------

struct Scene_Esc_Menu_UI_Callback : UI_Callback
{
	UI_Control_Data data_fullscreen;

	virtual UI_Control_Data* get_data( hash tag ) override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
};

// ----------------------------------------------------------------------------

struct Scene_Esc_Menu : Scene
{
	Scene_Esc_Menu();
	virtual ~Scene_Esc_Menu() override = default;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
};

}
