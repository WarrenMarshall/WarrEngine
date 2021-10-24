
struct Scene_Default final : Scene
{
	Texture_Asset* gradient = nullptr;
	Texture_Asset* tex_gauntlet = nullptr;
	Tween movement_tween, scale_tween, tilt_tween;

	Scene_Default();

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw() override;
};