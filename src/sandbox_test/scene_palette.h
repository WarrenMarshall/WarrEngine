
using namespace war;

struct Scene_Palette final : Scene
{
	Scene_Palette();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
