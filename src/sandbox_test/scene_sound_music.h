
using namespace war;

struct Scene_Sound_Music : Scene
{
	Texture_Asset* gradient = nullptr;

	Scene_Sound_Music();

	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void popped() override;
};
