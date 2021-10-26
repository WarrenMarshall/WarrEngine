
namespace war
{

struct Entity_Component
{
	Life_Cycle life_cycle;
	e_component_type component_type;
	[[nodiscard]] bool is_a( e_component_type type )
	{
		return ( component_type == type );
	}

	virtual void set_life_cycle( e_life_cycle lc );

	Transform _tform;
	[[nodiscard]] Transform* get_transform()
	{
		return &_tform;
	}

	[[nodiscard]] Vec2 get_pos()
	{
		return get_transform()->pos;
	}
	[[nodiscard]] f32 get_angle()
	{
		return get_transform()->angle;
	}
	[[nodiscard]] f32 get_scale()
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

	virtual void set_life_timer( i32 life_in_ms );

	// which collision mask(s) this entity is a part of
	i32 collision_mask = 0;

	// which collision mask(s) this entity will collide WITH
	i32 collides_with_mask = 0;

	virtual void set_collision_flags( i32 collision_mask, i32 collides_with );
	virtual void clear_collision_flags();
};

// ----------------------------------------------------------------------------

struct Sprite_Component final : Entity_Component
{
	Texture_Asset* texture = nullptr;
	f32 anim_offset = 0.f;

	struct
	{
		bool flip_x : 1 = false;
		bool flip_y : 1 = false;
	} flags;

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
		e_primitive_shape prim_shape = e_primitive_shape::rect;
		Rect rc = {};
		f32 radius = 0.f;
		Vec2 pos_offset = {};
	};

	std::vector<Shape> shapes;

	Primitive_Shape_Component() = delete;
	Primitive_Shape_Component( Entity* parent_entity );

	Entity_Component* add_shape( const e_primitive_shape prim_shape, const Rect& rc, const Vec2& pos_offset = Vec2::zero );
	Entity_Component* add_shape( const e_primitive_shape prim_shape, f32 radius, const Vec2& pos_offset = Vec2::zero );
	Entity_Component* add_shape( const e_primitive_shape prim_shape, const Vec2& pos_offset = Vec2::zero );

	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct Emitter_Component final : Entity_Component
{
	Emitter_Component() = default;
	Emitter_Component( Entity* parent_entity );

	Particle_Emitter emitter = {};

	Entity_Component* init( std::string_view params_tag );
	virtual void set_life_cycle( e_life_cycle lc ) override;
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

	void set_friction( f32 friction );
	void set_restitution( f32 restitution );
	void set_density( f32 density );

	virtual void set_collision_flags( i32 collision_mask, i32 collides_with ) override;
	virtual void clear_collision_flags() override;
};

// ----------------------------------------------------------------------------
// physics bodies

struct Box2D_Physics_Body_Component final : Entity_Component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	std::unordered_map<b2Fixture*, std::variant<b2PolygonShape, b2CircleShape, b2EdgeShape, b2ChainShape>> fixture_to_shape;

	// the primary body is the one that parent_entity will align
	// it's transform to each update.
	bool is_primary_body = false;

	Box2D_Physics_Body_Component() = delete;
	Box2D_Physics_Body_Component( Entity* parent_entity, e_physics_body_type type );
	virtual ~Box2D_Physics_Body_Component() override;

	void init_body();

	// all vec2 args below are relative to the position of the body
	//
	// this means that if the body is at the world origin, all vec2 args are
	// effectively world positions which is handy for things like static world
	// geometry.

	b2Fixture* add_fixture_box( hash tag, Vec2 pos, f32 w, f32 h );
	b2Fixture* add_fixture_box( hash tag, const Rect& rc );
	b2Fixture* add_fixture_circle( hash tag, Vec2 pos, f32 radius );
	b2Fixture* add_fixture_line( hash tag, Vec2 pos, Vec2 start, Vec2 end );
	b2Fixture* add_fixture_line_loop( hash tag, Vec2 pos, const std::vector<Vec2>& verts );
	b2Fixture* add_fixture_polygon( hash tag, Vec2 pos, const std::vector<Vec2>& verts );

	void add_physics_component_if_needed();

	virtual void set_collision_flags( i32 collision_mask, i32 collides_with ) override;
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

struct Collision_Body_Component : Entity_Component
{
	Collision_Body_Component() = delete;
	Collision_Body_Component( Entity* parent_entity );
	virtual ~Collision_Body_Component() = default;

	e_sc_prim_type prim_type = e_sc_prim_type::circle;

	[[nodiscard]] bool is_circle();
	[[nodiscard]] bool is_aabb();
	[[nodiscard]] bool is_polygon();

	struct
	{
		// if true, this is a platform collider. meaning the player can pass
		// through it from the bottom but not the top.
		bool is_platform : 1 = false;

		// if true, then any entities touching this collider need to move WITH this collider.
		bool is_sticky : 1 = false;

		// if true, then this collision body will draw itself during the regular
		// draw cycle. this is a convenience to remove the need to add a
		// Primitive_Shape_Component to every entity you want to see without
		// turning on debug drawing.
		bool draw_as_shape : 1 = false;
	} flags;

	// box
	Rect aabb = {};

	// circle
	f32 radius = 0.f;

	// verts
	std::vector<Vec2> verts = {};

	e_solidity solidity = e_solidity::solid;

	[[nodiscard]] bool is_solid()	{ return solidity == e_solidity::solid; }
	[[nodiscard]] bool is_sensor()	{ return solidity == e_solidity::sensor; }

	// world space values - these are computed as needed and are necessary for
	// testing collisions in the proper locations
	struct
	{
		Vec2 pos = {};
		Rect aabb = {};
		f32 radius = {};
		std::vector<Vec2> verts = {};
	} ws;

	void set_sensor_as_one_shot();
	void set_sensor_as_repeating( time_ms delay );
	void set_sensor_as_continuous();

	struct
	{
		e_sensor_type type = e_sensor_type::one_shot;

		// how long this sensor will wait between registering collision
		time_ms time_delay = 0;
		// the next time this sensor is available for collision
		time_ms time_next = 0;

		bool expired = false;

		bool is_one_shot() { return (type == e_sensor_type::one_shot ); }
		bool is_repeating() { return (type == e_sensor_type::repeating ); }
		bool is_continuous() { return (type == e_sensor_type::continuous ); }
		bool is_expired() { return expired; }

	} sensor;

	virtual void draw() override;
	void update_to_match_parent_transform();
	void set_as_box( f32 w, f32 h );
	void set_as_centered_box( f32 w, f32 h );
	void set_as_circle( f32 r );
	void set_as_polygon( std::vector<Vec2> verts );

	bool does_intersect_broadly( Collision_Body_Component* scc );
	virtual std::optional<collision::Pending_Collision> intersects_with_manifold( Collision_Body_Component* other );

	c2Circle as_c2_circle();
	c2AABB as_c2_aabb();
	c2Poly as_c2_poly();
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
	Collision_Body_Component* add_collision_body_from_object( const Tiled_Object& obj, e_solidity collider_type );
	void spawn_entities( Scene* scene, f_tile_map_spawn_entity func_callback );
};

}
