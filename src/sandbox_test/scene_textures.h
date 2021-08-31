
using namespace war;

struct Scene_Textures : Scene
{
	Texture_Asset* tex_crate = nullptr;
	Texture_Asset* grad_horizontal = nullptr;
	Texture_Asset* grad_vertical = nullptr;
	Texture_Asset* anim_player = nullptr;
	Texture_Asset* anim_coin = nullptr;

	float anim_offsets[ 3 ] = {};

	Scene_Textures();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
