
struct Scene_Default final : Scene
{
	Texture_Asset* gradient = nullptr;
	Texture_Asset* tex_hello_world = nullptr;
	Tween movement_tween, scale_tween, tilt_tween;

	Scene_Default();

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw() override;
};
