#pragma once

// ----------------------------------------------------------------------------

struct w_entity_component : i_life_cycle
{
	e_component_type type = component_type::invalid;
	w_transform tform;
	w_vec2 pos_interp;
	unsigned id = 0;

	std::optional<w_timer> life_timer = std::nullopt;

	// components have pointers to their entity parents for convenience
	w_entity* parent_entity = nullptr;

	// used for offsetting whatever the component needs to (0-1 range)
	float generic_offset = 0.0f;

	w_entity_component() = delete;
	w_entity_component( w_entity* parent_entity );
	virtual ~w_entity_component() = default;

	[[nodiscard]] virtual bool is_fully_dead();
	virtual void draw() {}
	virtual void update();

	virtual void set_life_timer( int life_in_ms );

	[[nodiscard]] virtual bool is_permanent();
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

	w_entity_component* init( const e_primitive_shape prim_shape, const w_color& color, const w_rect& rc );
	w_entity_component* init( const e_primitive_shape prim_shape, const w_color& color, const float radius );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : w_entity_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

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

	ec_sound() = delete;
	ec_sound( w_entity* parent_entity );

	w_entity_component* init( const std::string_view snd_tag );
	virtual void draw() override;
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

	struct
	{
		bool flipped_horizontally : 1;
		bool flipped_vertically : 1;
		bool flipped_diagonally : 1;
	};

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
