#pragma once

// ----------------------------------------------------------------------------

struct w_entity_component : i_life_cycle
{
	w_transform tform;
	w_entity* parent_entity = nullptr;
	hash tag;
	w_render_state_opt rs_opt;

	std::optional<w_timer> life_timer = std::nullopt;

	w_entity_component() = delete;
	w_entity_component( w_entity* parent_entity );
	virtual ~w_entity_component() = default;

	[[nodiscard]] virtual bool is_fully_dead();
	virtual void draw();
	virtual void update();
	virtual void play();
	virtual void stop();

	virtual void set_life_timer( int life_in_ms );

	[[nodiscard]] virtual bool is_permanent();
	[[nodiscard]] w_transform* get_transform();
};

// ----------------------------------------------------------------------------
// used as the base class for any entity components which are meant to always
// exist on the entity, like their transform.
//
// components that are assumed to always exist and be valid.

struct w_entity_component_permanent : w_entity_component
{
	w_entity_component_permanent( w_entity* parent_entity );

	[[nodiscard]] virtual bool is_fully_dead() override;
	[[nodiscard]] virtual bool is_permanent() override;
};

// ----------------------------------------------------------------------------

struct ec_transform : w_entity_component_permanent
{
	w_transform tform;

	ec_transform() = delete;
	ec_transform( w_entity* parent_entity );
};

// ----------------------------------------------------------------------------

struct ec_sprite : w_entity_component
{
	a_texture* texture = nullptr;
	float anim_offset = 0.0f;
	bool flip_x = false;
	bool flip_y = false;

	ec_sprite() = delete;
	ec_sprite( w_entity* parent_entity );

	w_entity_component* init( const std::string_view tex_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_primitive_shape : w_entity_component
{
	e_primitive_shape prim_shape = primitive_shape::rectangle;
	w_color color = w_color::white;
	w_rect rc;
	float radius = 0.0f;

	ec_primitive_shape() = delete;
	ec_primitive_shape( w_entity* parent_entity );

	w_entity_component* init( const e_primitive_shape prim_shape, const w_rect& rc );
	w_entity_component* init( const e_primitive_shape prim_shape, const float radius );
	w_entity_component* init( const e_primitive_shape prim_shape );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : w_entity_component
{
	w_particle_emitter emitter;

	ec_emitter() = delete;
	ec_emitter( w_entity* parent_entity );

	w_entity_component* init( const std::string_view params_tag );
	virtual void ilc_set( e_life_cycle life_cycle ) override;
	[[nodiscard]] virtual bool is_fully_dead() override;
	virtual void draw() override;
	virtual void update() override;
};

// ----------------------------------------------------------------------------

struct ec_sound : w_entity_component
{
	a_sound* snd = nullptr;

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
	ec_sound( w_entity* parent_entity );

	w_entity_component* init( const std::string_view snd_tag, bool one_shot, bool auto_play );
	virtual void update() override;
	virtual void play() override;
	virtual void stop() override;
};

// ----------------------------------------------------------------------------
// physics
//

struct ec_physics : w_entity_component
{
	ec_physics() = delete;
	ec_physics( w_entity* parent_entity );

	// which collision layer(s) this entity is a part of
	int collision_layer = 0;

	// which collision layer(s) this entity will collide WITH
	int collides_with = 0;

	void set_collision_flags( int collision_layer, int collides_with );
	void clear_collision_flags();

	[[nodiscard]] ec_b2d_body* get_primary_body();

	void set_friction( float friction );
	void set_restitution( float restitution );
	void set_density( float density );
};

// ----------------------------------------------------------------------------
// Box2D bodies
//

struct ec_b2d_body : w_entity_component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	// the primary body is the one that parent_entity will align
	// it's transform to each update.
	bool is_primary_body = false;

	ec_b2d_body() = delete;
	ec_b2d_body( w_entity* parent_entity );
	virtual ~ec_b2d_body() override;

	void init_body();

	// all vec2 args below are relative to the position of the body
	//
	// i.e. if the body is at the world origin, all vec2 args are
	// effectively world positions

	b2Fixture* add_fixture_box( const char* id, w_vec2 pos, float w, float h );
	b2Fixture* add_fixture_box( const char* id, const w_rect& rc );
	b2Fixture* add_fixture_circle( const char* id, w_vec2 pos, float radius );
	b2Fixture* add_fixture_line( const char* id, w_vec2 pos, w_vec2 start, w_vec2 end );
	b2Fixture* add_fixture_line_loop( const char* id, w_vec2 pos, const std::vector<w_vec2>& verts );
	b2Fixture* add_fixture_polygon( const char* id, w_vec2 pos, const std::vector<w_vec2>& verts );
};

// ----------------------------------------------------------------------------

struct ec_b2d_static : ec_b2d_body
{
	ec_b2d_static() = delete;
	ec_b2d_static( w_entity* parent_entity );
};

// ----------------------------------------------------------------------------
// NOTE :	entities can have a SINGLE dynamic body attached to them.

struct ec_b2d_dynamic : ec_b2d_body
{
	ec_b2d_dynamic() = delete;
	ec_b2d_dynamic( w_entity* parent_entity );
};

// ----------------------------------------------------------------------------
// kinematic bodies

struct ec_b2d_kinematic : ec_b2d_body
{
	ec_b2d_kinematic() = delete;
	ec_b2d_kinematic( w_entity* parent_entity );
};

// ----------------------------------------------------------------------------
// tile map

struct ec_tilemap_tile
{
	a_texture* texture = nullptr;
	int tileset_idx;
	w_pos pos = w_pos::zero;

	bool flipped_horizontally = false;
	bool flipped_vertically = false;
	bool flipped_diagonally = false;

	ec_tilemap_tile( int tileset_idx, w_pos pos, a_texture* texture );
};

struct ec_tilemap_layer
{
	std::vector<ec_tilemap_tile> tiles;
};

struct ec_tilemap : w_entity_component
{
	ec_tilemap() = delete;
	ec_tilemap( w_entity* parent_entity );

	std::vector<ec_tilemap_layer> tile_layers;

	// info about the tile map
	int width = 0;
	int height = 0;
	int tile_width = 0;
	int tile_height = 0;

	w_entity_component* init();
	virtual void draw() override;
	void load_from_disk( const char* tag, const std::vector<a_texture*>& texture_tiles, std::string_view level_filename );
};

// ----------------------------------------------------------------------------

struct ec_mesh : w_entity_component
{
	a_mesh* mesh = nullptr;

	ec_mesh() = delete;
	ec_mesh( w_entity* parent_entity );

	w_entity_component* init( const std::string_view mesh_tag );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

// #test - this component remains untested until we actually write a game that uses it
struct ec_follow_target : w_entity_component
{
	struct
	{
		w_entity* target = nullptr;
		e_follow_flags flags = follow_flags::xy_axis;

		// this is a multiplier against the default speed.
		// So 2.0 is twice as fast, 0.5f is twice as slow.
		float strength = 1.0f;

		// positions are snapped to the grid when set_pos is called on an entity,
		// so this is a scratch pad where we can keep the position of the following
		// entity in floating point form - aka not snapped to the grid. this means we
		// can smoothly interpolate towards the target each frame while still snapping
		// the entity itself to the grid for accurate rendering.
		w_vec2 pos = w_vec2::zero;

		std::optional<w_vec2> limits_x = std::nullopt;
		std::optional<w_vec2> limits_y = std::nullopt;
	} follow;

	ec_follow_target() = delete;
	ec_follow_target( w_entity* parent_entity );

	virtual void update() override;

	void set_follow_target( w_entity* entity_to_follow, e_follow_flags flags, float strength );
	void set_follow_limits_x( w_vec2 limits );
	void set_follow_limits_y( w_vec2 limits );
};