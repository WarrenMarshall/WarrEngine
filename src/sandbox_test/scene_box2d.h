
using namespace war;

struct Scene_Box2D final : Scene
{
	struct
	{
		ui16 all	= 0b00000001;
		ui16 world	= 0b00000010;
		ui16 ball	= 0b00000100;
	} coll_flags;

	Scene_Box2D();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	void spawn_ball_at( Vec2 world_pos );
	void spawn_box_at( Vec2 world_pos );
	virtual bool on_input( const Input_Event* evt ) override;
};
