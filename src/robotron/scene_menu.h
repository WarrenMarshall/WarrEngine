
struct Scene_Menu final : Scene
{
	Tween pulse_tween;

	Scene_Menu();

	virtual void pushed() override;
	virtual void draw_ui() override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
