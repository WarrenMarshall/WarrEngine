
struct scene_default final : scene
{
	Texture_Asset* gradient = nullptr;
	Texture_Asset* tex_hello_world = nullptr;
	Tween movement_tween, scale_tween, tilt_tween;

	scene_default();

	virtual void pushed() override;
	virtual void draw() override;
};
