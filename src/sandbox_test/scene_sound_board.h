
using namespace war;

struct scene_sound_board : scene
{
	texture_asset* gradient = nullptr;

	scene_sound_board();

	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void popped() override;
};
