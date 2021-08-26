
namespace war
{

// a scene that can be pushed onto the top of the stack that removes all control
// from the user and allows for effects like scene transtions or anything else
// you want to do on a node timeline.

struct scene_timeline final : scene
{
	scene_timeline();

	timeline_nodes fx_timeline;

	virtual void pushed() override;
	virtual void update() override;
};

}
