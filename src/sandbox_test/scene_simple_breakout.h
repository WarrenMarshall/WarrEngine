
using namespace war;

// ----------------------------------------------------------------------------

struct e_ball final : entity
{
	virtual bool on_collided( simple_collision::pending_collision& coll ) override;
};

// ----------------------------------------------------------------------------

struct e_paddle final : entity
{
	virtual bool on_collided( simple_collision::pending_collision& coll ) override;
};

// ----------------------------------------------------------------------------
struct scene_simple_breakout final : scene
{
	scene_simple_breakout();

	entity* paddle = nullptr;
	entity* world_geo = nullptr;

	void spawn_ball();
	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const input_event* evt ) override;
	virtual bool on_input_held( const input_event* evt ) override;
	virtual bool on_input_motion( const input_event* evt ) override;
};
