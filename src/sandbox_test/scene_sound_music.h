
using namespace war;

struct Scene_Sound_Music final : Scene
{
	Scene_Sound_Music();

	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void popped() override;
};
