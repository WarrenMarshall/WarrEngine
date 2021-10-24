
struct Scene_Gameplay final : Scene
{
	Texture_Asset* gradient = nullptr;
	Texture_Asset* tex_robotron = nullptr;
	Tween movement_tween, scale_tween, tilt_tween;

	Scene_Gameplay();

	virtual void pushed() override;
	virtual void draw() override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
