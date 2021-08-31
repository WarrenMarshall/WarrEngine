
using namespace war;

struct Scene_Tweens_Timers : Scene
{
	Texture_Asset* tween_tex = nullptr;

	Tween tween_linear;
	Tween tween_sin;
	Tween tween_quad;
	Tween tween_circular;
	Tween tween_bounce;
	Tween tween_back;
	Tween tween_cubic;
	Tween tween_elastic;
	Tween tween_exponential;
	Tween tween_quartic;
	Tween tween_quintic;

	Timer timer_01;
	Timer timer_02;

	Scene_Tweens_Timers();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
