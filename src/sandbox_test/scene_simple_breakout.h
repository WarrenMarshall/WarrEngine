
using namespace war;

// ----------------------------------------------------------------------------

struct e_ball final : Entity
{
	virtual bool on_collided( simple_collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------

struct e_paddle final : Entity
{
	virtual bool on_collided( simple_collision::Pending_Collision& coll ) override;
};

// ----------------------------------------------------------------------------
struct scene_simple_breakout final : scene
{
	scene_simple_breakout();

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
