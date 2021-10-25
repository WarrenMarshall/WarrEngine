
using namespace war;

// ----------------------------------------------------------------------------

struct E_Breakout_Paddle final : Entity
{
	virtual bool on_collided( collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------
struct Scene_Breakout final : Scene
{
	struct
	{
		ui16 ball	= 0b00000001;
		ui16 paddle = 0b00000010;
		ui16 geo	= 0b00000100;
	} coll_flags;

	Scene_Breakout();

	Entity* paddle = nullptr;
	Entity* world_geo = nullptr;

	void spawn_ball();
	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_held( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
