
namespace war
{

struct scene_controller_required final : scene
{
	texture_asset* tex_game_controller = nullptr;
	tween anim_tween;
	std::vector<std::string> msg_false, msg_true;

	virtual void pushed() override;
	virtual void popped() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
};

}
