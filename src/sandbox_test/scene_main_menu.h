
using namespace war;

struct scene_main_menu : scene
{
	scene_main_menu();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};