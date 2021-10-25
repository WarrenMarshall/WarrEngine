
using namespace war;

struct E_Jump_Pad final : Entity
{
	Sprite_Component* sprite_component = nullptr;
	Texture_Asset* tex_jump_pad_up = nullptr;
	Texture_Asset* tex_jump_pad_down = nullptr;

	time_ms time_reset = 0;

	E_Jump_Pad();
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct E_Player final : Entity
{
	virtual bool on_touching_begin( Collision_Body_Component* sensor ) override;
};

// ----------------------------------------------------------------------------
struct Scene_Simple_Platformer final : Scene
{
	struct
	{
		ui16 player		= 0b00000001;
		ui16 geo		= 0b00000010;
		ui16 jump_pad	= 0b00000100;
		ui16 mover		= 0b00001000;
	} coll_flags;

	Entity* world = nullptr;
	Entity* player = nullptr;
	Entity* mover = nullptr;

	Timeline_Of_Nodes fx_red_alert;

	Scene_Simple_Platformer();

	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void pushed() override;
	virtual void update() override;

	virtual bool on_input_motion( const Input_Event* evt ) override;
	virtual bool on_input_pressed( const Input_Event* evt ) override;
};
