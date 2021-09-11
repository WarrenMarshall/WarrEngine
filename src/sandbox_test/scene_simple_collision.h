
using namespace war;

struct Scene_Simple_Collision final : Scene
{
	struct
	{
		uint16_t player			= 0b00000001;
		uint16_t geo			= 0b00000010;
		uint16_t dynamic_object	= 0b00000100;
	} coll_flags;

	Scene_Simple_Collision();

	Entity* player = nullptr;
	Entity* world_geo = nullptr;

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
	void spawn_ball_at( Vec2 world_pos );
	void spawn_box_at( Vec2 world_pos );
};
