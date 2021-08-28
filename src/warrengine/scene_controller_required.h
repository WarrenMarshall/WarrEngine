
namespace war
{

struct Scene_Controller_Required final : scene
{
	Texture_Asset* tex_game_controller = nullptr;
	Tween anim_tween;
	std::vector<std::string> msg_false, msg_true;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
};

}
