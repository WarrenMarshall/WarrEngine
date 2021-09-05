
using namespace war;

struct E_Jump_Pad : Entity
{
	Sprite_Component* sprite_component = nullptr;
	Texture_Asset* tex_jump_pad_up = nullptr;
	Texture_Asset* tex_jump_pad_down = nullptr;

	time_ms time_reset = 0;

	E_Jump_Pad();
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct E_Player : Entity
{
	virtual bool on_touching_begin( Simple_Collision_Body* sensor ) override;
};

// ----------------------------------------------------------------------------
struct Scene_Simple_Platformer : Scene
{
	struct
	{
		uint16_t player		= 0b00000001;
		uint16_t geo		= 0b00000010;
		uint16_t jump_pad	= 0b00000100;
		uint16_t mover		= 0b00001000;
	} coll_flags;

	Entity* world = nullptr;
	Entity* player = nullptr;
	Entity* mover = nullptr;

	Timeline_Nodes fx_red_alert;

	Scene_Simple_Platformer();

	virtual void draw() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
