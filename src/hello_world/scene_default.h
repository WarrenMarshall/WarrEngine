
struct scene_default : war::scene
{
	war::texture_asset* gradient = nullptr;
	war::texture_asset* tex_hello_world = nullptr;
	war::tween movement_tween, scale_tween, tilt_tween;

	scene_default();

	virtual void pushed() override;
	virtual void draw() override;
};
