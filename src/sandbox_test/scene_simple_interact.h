
using namespace war;

struct Scene_Simple_Interact final : Scene
{
	struct
	{
		ui16 player	= 0b00000001;
		ui16 geo	= 0b00000010;
		ui16 npc	= 0b00000100;
	} coll_flags;

	Scene_Simple_Interact();

	Entity* player = nullptr;
	Entity* hit_marker = nullptr;
	Entity* world_geo = nullptr;

	bool b_show_ray = false;
	Vec2 ray_dir = {};

	Entity* spawn_entity();

	virtual void pushed() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	void reset_collision_trace_results();

	virtual bool on_input_pressed( const Input_Event* evt ) override;
	virtual bool on_input_motion( const Input_Event* evt ) override;
};
