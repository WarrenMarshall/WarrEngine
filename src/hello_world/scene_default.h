
struct scene_default final : scene
{
	texture_asset* gradient = nullptr;
	texture_asset* tex_hello_world = nullptr;
	tween movement_tween, scale_tween, tilt_tween;

	scene_default();

	virtual void pushed() override;
	virtual void draw() override;
};
