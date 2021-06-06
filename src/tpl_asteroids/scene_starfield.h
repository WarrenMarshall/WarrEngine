
using namespace war;

struct scene_starfield : scene
{
	texture_asset* gradient = nullptr;

	scene_starfield();

	virtual void pushed() override;
	virtual void draw() override;
};
