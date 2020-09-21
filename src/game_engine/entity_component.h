#pragma once

// ----------------------------------------------------------------------------

struct w_entity_component : i_life_cycle, i_transform
{
	e_component_type type = component_type::invalid;
	w_vec2 pos_interp;

	std::unique_ptr<w_timer> life_timer = nullptr;

	// components have pointers to their entity parents for convenience
	w_entity* parent_entity = nullptr;

	// used for offsetting whatever the component needs to (0-1 range)
	float generic_offset = 0.0f;

	w_entity_component() = delete;
	w_entity_component( w_entity* parent_entity );
	virtual ~w_entity_component() {};

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
//

struct ec_b2d_body : w_entity_component
{
	b2BodyType body_type = b2_staticBody;
	b2Body* body = nullptr;

	ec_b2d_body() = delete;
	ec_b2d_body( w_entity* parent_entity );
	virtual ~ec_b2d_body() override;

	void init_body();

	// offset - an offset from the position of the entity. this allows for things
	//			like sensors that need to be at a character's feet.
	b2Fixture* add_fixture_box( w_vec2 offset, float width, float height );
	b2Fixture* add_fixture_circle( w_vec2 offset, float radius );
	b2Fixture* add_fixture_line( w_vec2 offset, float width, float height );
	b2Fixture* add_fixture_chain( w_vec2 offset, const std::vector<w_vec2>& verts );

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
