
using namespace war;

struct scene_tweens : scene
{
	texture_asset* tween_tex = nullptr;

	tween tween_linear;
	tween tween_sin;
	tween tween_quad;
	tween tween_circular;
	tween tween_bounce;
	tween tween_back;
	tween tween_cubic;
	tween tween_elastic;
	tween tween_exponential;
	tween tween_quartic;
	tween tween_quintic;

	timer timer_01;
	timer timer_02;

	scene_tweens();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
