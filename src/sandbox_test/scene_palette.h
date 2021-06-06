
using namespace war;

struct scene_palette : scene
{
	scene_palette();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
