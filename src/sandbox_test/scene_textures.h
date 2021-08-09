
using namespace war;

struct scene_textures : scene
{
	texture_asset* tex_crate = nullptr;
	texture_asset* grad_horizontal = nullptr;
	texture_asset* grad_vertical = nullptr;
	texture_asset* anim_player = nullptr;
	texture_asset* anim_coin = nullptr;

	float anim_offsets[ 3 ] = {};

	scene_textures();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
