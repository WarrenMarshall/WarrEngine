
struct Scene_Background final : Scene
{
	Texture_Asset* gradient = nullptr;
	Texture_Asset* tex_robotron = nullptr;
	Tween movement_tween, scale_tween, tilt_tween;

	Scene_Background();

	virtual void pushed() override;
	virtual void draw() override;
};
