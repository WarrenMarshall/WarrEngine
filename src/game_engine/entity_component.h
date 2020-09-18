#pragma once

struct w_entity_component : i_lifecycle, i_transform
{
	//e_component_type type = component_type::invalid;
	w_vec2 pos_interp;

	std::unique_ptr<w_timer> life_timer = nullptr;

	// components have pointers to their entity parents for convenience
	w_entity* parent_entity = nullptr;

	// used for offsetting whatever the component needs to (0-1 range)
	float generic_offset = 0.0f;

	w_entity_component() = delete;
	w_entity_component( w_entity* parent_entity );
	virtual ~w_entity_component() = default;

	virtual bool is_fully_dead();
	virtual void draw() {}
	virtual void update();
	virtual void post_spawn() {}

	virtual void set_life_timer( int life_in_ms );
};

// ----------------------------------------------------------------------------

struct ec_sprite : w_entity_component
{
	a_subtexture* subtex = nullptr;
	bool flip_x = false, flip_y = false;

	ec_sprite() = delete;
	ec_sprite( w_entity* parent_entity );

	w_entity_component* init( const std::string_view subtex_name );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : w_entity_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

	ec_emitter() = delete;
	ec_emitter( w_entity* parent_entity );

	w_entity_component* init( const std::string_view params_name );
	virtual void set_life_cycle( e_life_cycle life_cycle ) override;
	virtual bool is_fully_dead() override;
	virtual void draw() override;
	virtual void update() override;
	virtual void post_spawn() override;
};

// ----------------------------------------------------------------------------

struct ec_sound : w_entity_component
{
	a_sound* snd = nullptr;

	ec_sound() = delete;
	ec_sound( w_entity* parent_entity );

	w_entity_component* init( const std::string_view snd_name );
	virtual void draw() override;
};

// ----------------------------------------------------------------------------
// Box2D bodies

struct ec_b2d_body : w_entity_component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	ec_b2d_body() = delete;
	ec_b2d_body( w_entity* parent_entity );

	ec_b2d_body* init_as_circle( float radius );
	ec_b2d_body* init_as_circle( const w_vec2& pos, float radius );

	ec_b2d_body* init_as_box( float width, float height );
	ec_b2d_body* init_as_box( const w_vec2& pos, float width, float height );

	ec_b2d_body* init_as_line( const w_vec2& start, const w_vec2& end );
	ec_b2d_body* init_as_line( const w_vec2& pos, const w_vec2& start, const w_vec2& end );

	virtual void draw() override;
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

// #box2d - this might be useful later as a "simple" collision checker
/*
struct ec_collider : w_entity_component
{
	C2_TYPE c2type = C2_TYPE_NONE;
	float radius = 0.0f;
	w_rect box;

	variant_collider_types collision_object = {};

	ec_collider() = delete;
	ec_collider( w_entity* parent_entity );

	void push_outside( const c2Manifold& hit );
	w_entity_component* init_as_circle( float radius );
	w_entity_component* init_as_box( w_rect box );
	variant_collider_types get_collider();
	virtual void draw() override;
};
*/

// ----------------------------------------------------------------------------
// a force that pushes on the entity, and never decays
//
// good for things that need to be constantly moving, like the ball in pong.

struct ec_force_constant : w_entity_component
{
	float angle = 0.0f;
	float strength = 0.0f;

	ec_force_constant() = delete;
	ec_force_constant( w_entity* parent_entity );

	ec_force_constant* init( float angle, float strength );
};

// ----------------------------------------------------------------------------
// a force that pushes on the entity, and decays to zero over a
// specific number of milliseconds
//
// good for temporary forces like jumps or getting knocked back.

struct ec_force_multiplier : w_entity_component
{
	float _strength = 0.0f;
	float strength = 0.0f;
	float _lifetime_in_ms = 0.0f;
	float lifetime_in_ms = 0.0f;

	ec_force_multiplier() = delete;
	ec_force_multiplier( w_entity* parent_entity );

	ec_force_multiplier* init( float strength, float lifetime_in_ms );

	virtual void update() override;
};

// ----------------------------------------------------------------------------
// a force that pushes on the entity, getting stronger every second until
// a maximum is reached.
//
// good for movement where the entity speeds up and slows down smoothly.

struct ec_force_dir_accum : w_entity_component
{
	// current direction
	float angle = 0.0f;
	// current strength
	float strength = 0.0f;
	// how much we accumulate per second
	float strength_per_sec = 0.0f;
	// strength maximum
	float strength_max = 0.0f;

	ec_force_dir_accum() = delete;
	ec_force_dir_accum( w_entity* parent_entity );

	ec_force_dir_accum* init( float angle, float strength_per_sec, float strength_max );
	void add_impulse();
	void decay();
};
