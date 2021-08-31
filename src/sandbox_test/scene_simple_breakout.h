
using namespace war;

// ----------------------------------------------------------------------------

struct E_Breakout_Ball : Entity
{
	virtual bool on_collided( simple_collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------

struct E_Breakout_Paddle : Entity
{
	virtual bool on_collided( simple_collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------
struct Scene_Simple_Breakout : Scene
{
	Scene_Simple_Breakout();

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
