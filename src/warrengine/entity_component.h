
namespace war
{

struct Entity_Component
{
	Life_Cycle life_cycle;

	virtual void set_life_cycle( e_life_cycle_t lc );

	Transform _tform;
	[[nodiscard]] Transform* get_transform()
	{
		return &_tform;
	}

	[[nodiscard]] Vec2 get_pos()
	{
		return get_transform()->pos;
	}
	[[nodiscard]] float_t get_angle()
	{
		return get_transform()->angle;
	}
	[[nodiscard]] float_t get_scale()
	{
		return get_transform()->scale;
	}

	Entity* parent_entity = nullptr;
	hash tag = hash_none;
	Render_State_Optional rs_opt;

	std::optional<Timer> life_timer = std::nullopt;

	// some components have things they want to do as a last construction step.
	// checking this value at the top of their Update() functions gives them the
	// chance to do that.
	//
	// i.e. particle systems that want to warm up

	bool was_finalized = false;

	Entity_Component() = delete;
	Entity_Component( Entity* parent_entity );
	virtual ~Entity_Component() = default;

	[[nodiscard]] virtual bool is_fully_dead();
	virtual void draw();
	virtual void pre_update();
	virtual void update();
	virtual void post_update();
	virtual void play();
	virtual void stop();

	virtual void set_life_timer( int32_t life_in_ms );

	// which collision mask(s) this entity is a part of
	int32_t collision_mask = 0;

	// which collision mask(s) this entity will collide WITH
	int32_t collides_with_mask = 0;

	virtual void set_collision_flags( int32_t collision_mask, int32_t collides_with );
	virtual void clear_collision_flags();
};

// ----------------------------------------------------------------------------

struct Sprite_Component final : Entity_Component
{
	Texture_Asset* texture = nullptr;
	float_t anim_offset = 0.f;
	bool flip_x = false;
	bool flip_y = false;

	Sprite_Component() = delete;
	Sprite_Component( Entity* parent_entity );

	Entity_Component* init( std::string_view tex_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct Primitive_Shape_Component final : Entity_Component
{
	struct Shape final
	{
		e_primitive_shape_t prim_shape = e_primitive_shape::rect;
		Rect rc = {};
		float_t radius = 0.f;
		Vec2 pos_offset = {};
	};

	std::vector<Shape> shapes;

	Primitive_Shape_Component() = delete;
	Primitive_Shape_Component( Entity* parent_entity );

	Entity_Component* add_shape( const e_primitive_shape_t prim_shape, const Rect& rc, const Vec2& pos_offset = Vec2::zero );
	Entity_Component* add_shape( const e_primitive_shape_t prim_shape, float_t radius, const Vec2& pos_offset = Vec2::zero );
	Entity_Component* add_shape( const e_primitive_shape_t prim_shape, const Vec2& pos_offset = Vec2::zero );

	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct Emitter_Component final : Entity_Component
{
	Emitter_Component() = default;
	Emitter_Component( Entity* parent_entity );

	Particle_Emitter emitter = {};

	Entity_Component* init( std::string_view params_tag );
	virtual void set_life_cycle( e_life_cycle_t lc ) override;
	[[nodiscard]] virtual bool is_fully_dead() override;
	virtual void draw() override;
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct Sound_Component final : Entity_Component
{
	Sound_Asset* snd = nullptr;

	// general usage cases:
	//
	// music
	//		- one_shot = false, auto_play = false
	// sound
	//		- one_shot = true, auto_play = false
	// sound_fx (i.e. part of a w_entity_transient)
	//		- one_shot = true, auto_play = true

	// indicates that this sound is played exactly once. after
	// it plays, the component will mark itself as dying so it
	// can be cleaned up.
	//
	// setting this to false is useful if you want to be able
	// to trigger a sound over and over again on the same
	// entity component;
	bool one_shot = false;

	// means that the sound will automatically start playing the next
	// time the "update" function is called
	bool auto_play = false;

	Sound_Component() = delete;
	Sound_Component( Entity* parent_entity );

	Entity_Component* init( std::string_view snd_tag, bool one_shot, bool auto_play );
	virtual void update() override;
	virtual void play() override;
	virtual void stop() override;
};

// ----------------------------------------------------------------------------
// physics

struct Box2D_Physics_Component final : Entity_Component
{
	Box2D_Physics_Component() = delete;
	Box2D_Physics_Component( Entity* parent_entity );

	[[nodiscard]] Box2D_Physics_Body_Component* get_primary_body();

	void set_friction( float_t friction );
	void set_restitution( float_t restitution );
	void set_density( float_t density );

	virtual void set_collision_flags( int32_t collision_mask, int32_t collides_with ) override;
	virtual void clear_collision_flags() override;
};

// ----------------------------------------------------------------------------
// physics bodies

struct Box2D_Physics_Body_Component : Entity_Component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	std::unordered_map<b2Fixture*, std::variant<b2PolygonShape, b2CircleShape, b2EdgeShape, b2ChainShape>> fixture_to_shape;

	// the primary body is the one that parent_entity will align
	// it's transform to each update.
	bool is_primary_body = false;

	Box2D_Physics_Body_Component() = delete;
	Box2D_Physics_Body_Component( Entity* parent_entity );
	virtual ~Box2D_Physics_Body_Component() override;

	void init_body();

	// all vec2 args below are relative to the position of the body
	//
	// this means that if the body is at the world origin, all vec2 args are
	// effectively world positions which is handy for things like static world
	// geometry.

	b2Fixture* add_fixture_box( hash tag, Vec2 pos, float_t w, float_t h );
	b2Fixture* add_fixture_box( hash tag, const Rect& rc );
	b2Fixture* add_fixture_circle( hash tag, Vec2 pos, float_t radius );
	b2Fixture* add_fixture_line( hash tag, Vec2 pos, Vec2 start, Vec2 end );
	b2Fixture* add_fixture_line_loop( hash tag, Vec2 pos, const std::vector<Vec2>& verts );
	b2Fixture* add_fixture_polygon( hash tag, Vec2 pos, const std::vector<Vec2>& verts );

	void add_physics_component_if_needed();

	virtual void set_collision_flags( int32_t collision_mask, int32_t collides_with ) override;
};

// ----------------------------------------------------------------------------

struct Box2D_Static_Body_Component final : Box2D_Physics_Body_Component
{
	Box2D_Static_Body_Component() = delete;
	Box2D_Static_Body_Component( Entity* parent_entity );
};

// ----------------------------------------------------------------------------
// NOTE :	entities can have a SINGLE dynamic body attached to them.

struct Box2D_Dynamic_Body_Component final : Box2D_Physics_Body_Component
{
	Box2D_Dynamic_Body_Component() = delete;
	Box2D_Dynamic_Body_Component( Entity* parent_entity );
};

// ----------------------------------------------------------------------------
// kinematic bodies

struct Box2D_Kinematic_Body_Component final : Box2D_Physics_Body_Component
{
	Box2D_Kinematic_Body_Component() = delete;
	Box2D_Kinematic_Body_Component( Entity* parent_entity );
};

// ----------------------------------------------------------------------------

struct Mesh_Component final : Entity_Component
{
	Mesh_Asset* mesh = nullptr;

	Mesh_Component() = delete;
	Mesh_Component( Entity* parent_entity );

	Entity_Component* init( std::string_view mesh_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct Simple_Collision_Body : Entity_Component
{
	Simple_Collision_Body() = delete;
	Simple_Collision_Body( Entity* parent_entity );
	virtual ~Simple_Collision_Body() = default;

	e_sc_prim_type_t type = e_sc_prim_type::circle;

	bool is_platform = false;

	// if true, then any entities touching this sensor need to move WITH the sensor.
	bool is_sticky = false;

	// box
	Rect aabb = {};

	// circle
	float_t radius = 0.f;

	// verts
	std::vector<Vec2> verts = {};

	e_sc_body_collider_type_t collider_type = e_sc_body_collider_type::solid;

	[[nodiscard]] bool is_solid()
	{
		return collider_type == e_sc_body_collider_type::solid;
	}
	[[nodiscard]] bool is_sensor()
	{
		return collider_type == e_sc_body_collider_type::sensor;
	}

	struct
	{
		Vec2 pos = {};
		Rect aabb = {};
		float_t radius = {};
		std::vector<Vec2> verts = {};
	} ws;

	virtual void draw() override;
	void update_to_match_parent_transform();
	void set_as_box( float_t w, float_t h );
	void set_as_centered_box( float_t w, float_t h );
	void set_as_circle( float_t r );
	void set_as_polygon( std::vector<Vec2> verts );
	void set_body_collider_type( e_sc_body_collider_type_t type );

	bool intersects_with_quick( Simple_Collision_Body* scc );
	virtual std::optional<simple_collision::Pending_Collision> intersects_with_manifold( Simple_Collision_Body* other );

	c2Circle as_simple_circle();
	c2AABB as_simple_aabb();
	c2Poly as_simple_poly();
	c2Circle get_bounds_as_simple_circle();
};

// ----------------------------------------------------------------------------

struct Simple_Collision_Platform_Body final : Simple_Collision_Body
{
	Simple_Collision_Platform_Body() = delete;
	Simple_Collision_Platform_Body( Entity* parent_entity );

	virtual std::optional<simple_collision::Pending_Collision> intersects_with_manifold( Simple_Collision_Body* other ) override;
};

// ----------------------------------------------------------------------------

struct Tile_Map_Component final : Entity_Component
{
	Tile_Map_Component() = delete;
	Tile_Map_Component( Entity* parent_entity );

	Tile_Set_Asset* tile_set = nullptr;
	Tile_Map_Asset* tile_map = nullptr;

	virtual void draw() override;

	void init( std::string_view tile_set_name, std::string_view tile_map_name );
	void spawn_entities( Scene* scene, f_tile_map_spawn_entity func_callback );
};

}
