
namespace war
{

struct entity_component
{
	life_cycle_mgr life_cycle;

	virtual void set_life_cycle( e_life_cycle lc );

	transform _tform;
	[[nodiscard]] transform* get_transform()
	{
		return &_tform;
	}

	[[nodiscard]] vec2 get_pos()
	{
		return get_transform()->pos;
	}
	[[nodiscard]] float get_angle()
	{
		return get_transform()->angle;
	}
	[[nodiscard]] float get_scale()
	{
		return get_transform()->scale;
	}

	entity* parent_entity = nullptr;
	hash tag = hash_none;
	render_state_optional rs_opt;

	std::optional<timer> life_timer = std::nullopt;

	// some components have things they want to do as a last construction step.
	// checking this value at the top of their Update() functions gives them the
	// chance to do that.
	//
	// i.e. particle systems that want to warm up

	bool was_finalized = false;

	entity_component() = delete;
	entity_component( entity* parent_entity );
	virtual ~entity_component() = default;

	[[nodiscard]] virtual bool is_fully_dead();
	virtual void draw();
	virtual void pre_update();
	virtual void update();
	virtual void post_update();
	virtual void play();
	virtual void stop();

	virtual void set_life_timer( int life_in_ms );

	// which collision mask(s) this entity is a part of
	int collision_mask = 0;

	// which collision mask(s) this entity will collide WITH
	int collides_with_mask = 0;

	virtual void set_collision_flags( int collision_mask, int collides_with );
	virtual void clear_collision_flags();
};

// ----------------------------------------------------------------------------

struct ec_sprite : entity_component
{
	texture_asset* texture = nullptr;
	float anim_offset = 0.f;
	bool flip_x = false;
	bool flip_y = false;

	ec_sprite() = delete;
	ec_sprite( entity* parent_entity );

	entity_component* init( std::string_view tex_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_primitive_shape : entity_component
{
	struct shape_def
	{
		e_primitive_shape prim_shape = primitive_shape::rect;
		rect rc = {};
		float radius = 0.f;
		vec2 pos_offset = {};
	};

	std::vector<shape_def> shapes;

	ec_primitive_shape() = delete;
	ec_primitive_shape( entity* parent_entity );

	entity_component* add_shape( const e_primitive_shape prim_shape, const rect& rc, const vec2& pos_offset = vec2::zero );
	entity_component* add_shape( const e_primitive_shape prim_shape, float radius, const vec2& pos_offset = vec2::zero );
	entity_component* add_shape( const e_primitive_shape prim_shape, const vec2& pos_offset = vec2::zero );

	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : entity_component
{
	ec_emitter() = default;
	ec_emitter( entity* parent_entity );

	particle_emitter emitter = {};

	entity_component* init( std::string_view params_tag );
	virtual void set_life_cycle( e_life_cycle lc ) override;
	[[nodiscard]] virtual bool is_fully_dead() override;
	virtual void draw() override;
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct ec_sound : entity_component
{
	sound_asset* snd = nullptr;

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

	ec_sound() = delete;
	ec_sound( entity* parent_entity );

	entity_component* init( std::string_view snd_tag, bool one_shot, bool auto_play );
	virtual void update() override;
	virtual void play() override;
	virtual void stop() override;
};

// ----------------------------------------------------------------------------
// physics

struct ec_box2d_physics : entity_component
{
	ec_box2d_physics() = delete;
	ec_box2d_physics( entity* parent_entity );

	[[nodiscard]] ec_box2d_physics_body* get_primary_body();

	void set_friction( float friction );
	void set_restitution( float restitution );
	void set_density( float density );

	virtual void set_collision_flags( int collision_mask, int collides_with ) override;
	virtual void clear_collision_flags() override;
};

// ----------------------------------------------------------------------------
// physics bodies

struct ec_box2d_physics_body : entity_component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	std::unordered_map<b2Fixture*, std::variant<b2PolygonShape, b2CircleShape, b2EdgeShape, b2ChainShape>> fixture_to_shape;

	// the primary body is the one that parent_entity will align
	// it's transform to each update.
	bool is_primary_body = false;

	ec_box2d_physics_body() = delete;
	ec_box2d_physics_body( entity* parent_entity );
	virtual ~ec_box2d_physics_body() override;

	void init_body();

	// all vec2 args below are relative to the position of the body
	//
	// i.e. if the body is at the world origin, all vec2 args are
	// effectively world positions

	b2Fixture* add_fixture_box( hash tag, vec2 pos, float w, float h );
	b2Fixture* add_fixture_box( hash tag, const rect& rc );
	b2Fixture* add_fixture_circle( hash tag, vec2 pos, float radius );
	b2Fixture* add_fixture_line( hash tag, vec2 pos, vec2 start, vec2 end );
	b2Fixture* add_fixture_line_loop( hash tag, vec2 pos, const std::vector<vec2>& verts );
	b2Fixture* add_fixture_polygon( hash tag, vec2 pos, const std::vector<vec2>& verts );

	void add_physics_component_if_needed();

	virtual void set_collision_flags( int collision_mask, int collides_with ) override;
};

// ----------------------------------------------------------------------------

struct ec_box2d_static_physics_body : ec_box2d_physics_body
{
	ec_box2d_static_physics_body() = delete;
	ec_box2d_static_physics_body( entity* parent_entity );
};

// ----------------------------------------------------------------------------
// NOTE :	entities can have a SINGLE dynamic body attached to them.

struct ec_box2d_dynamic_physics_body : ec_box2d_physics_body
{
	ec_box2d_dynamic_physics_body() = delete;
	ec_box2d_dynamic_physics_body( entity* parent_entity );
};

// ----------------------------------------------------------------------------
// kinematic bodies

struct ec_box2d_kinematic_physics_body : ec_box2d_physics_body
{
	ec_box2d_kinematic_physics_body() = delete;
	ec_box2d_kinematic_physics_body( entity* parent_entity );
};

// ----------------------------------------------------------------------------

struct ec_mesh : entity_component
{
	mesh_asset* mesh = nullptr;

	ec_mesh() = delete;
	ec_mesh( entity* parent_entity );

	entity_component* init( std::string_view mesh_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_simple_collision_body : entity_component
{
	ec_simple_collision_body() = delete;
	ec_simple_collision_body( entity* parent_entity );

	e_sc_prim_type type = sc_prim_type::circle;

	bool is_platform = false;

	// box
	rect aabb = {};

	// circle
	float radius = 0.f;

	// verts
	std::vector<vec2> verts = {};

	e_sc_body_collider_type collider_type = sc_body_collider_type::solid;

	struct
	{
		vec2 pos = {};
		rect aabb = {};
		float radius = {};
		std::vector<vec2> verts = {};
	} ws;

	virtual void draw() override;
	void update_to_match_parent_transform();
	void set_as_box( float w, float h );
	void set_as_centered_box( float w, float h );
	void set_as_circle( float r );
	void set_as_polygon( std::vector<vec2> verts );
	void set_body_collider_type( e_sc_body_collider_type type );

	bool intersects_with( ec_simple_collision_body* scc );
	virtual std::optional<simple_collision::pending_collision> intersects_with_manifold( ec_simple_collision_body* other );

	c2Circle as_simple_circle();
	c2AABB as_simple_aabb();
	c2Poly as_simple_poly();
	c2Circle get_bounds_as_simple_circle();
};

struct ec_simple_collision_body_platform : ec_simple_collision_body
{
	ec_simple_collision_body_platform() = delete;
	ec_simple_collision_body_platform( entity* parent_entity );

	virtual std::optional<simple_collision::pending_collision> intersects_with_manifold( ec_simple_collision_body* other ) override;

	vec2 closest_point;
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_tile_map : entity_component
{
	ec_tile_map() = delete;
	ec_tile_map( entity* parent_entity );

	tile_set_asset* tile_set = nullptr;
	tile_map_asset* tile_map = nullptr;

	virtual void draw() override;

	void init( std::string_view tile_set_name, std::string_view tile_map_name );
	void spawn_entities( scene* scene, f_tile_map_spawn_entity func_callback );
};

}
