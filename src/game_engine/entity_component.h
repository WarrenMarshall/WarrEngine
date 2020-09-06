#pragma once

struct w_entity_component : i_lifecycle, i_transform
{
	e_component_type type = component_type::invalid;
	w_vec2 pos_interp;

	// if this boolean is true OR there is time remaining on
	// the clock, this component is active.
	bool active = true;
	float active_time_remaining_ms = 0;

	// components have pointers to their entity parents for convenience
	w_entity* parent_entity = nullptr;

	// used for offsetting whatever the component needs to (0-1 range)
	float generic_offset = 0.0f;

	w_entity_component() = delete;
	w_entity_component( w_entity* parent_entity );
	virtual ~w_entity_component() = default;

	bool is_active();

	virtual bool is_fully_dead();
	virtual void draw() {}
	virtual void update();
	virtual void post_spawn() {}
};

// ----------------------------------------------------------------------------

struct ec_sprite : w_entity_component
{
	a_subtexture* subtex = nullptr;
	bool flip_x = false, flip_y = false;

	ec_sprite() = delete;
	ec_sprite( w_entity* parent_entity );

	w_entity_component* init( const std::string_view subtex_name );
	void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : w_entity_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

	ec_emitter() = delete;
	ec_emitter( w_entity* parent_entity );

	w_entity_component* init( const std::string_view params_name );
	void set_life_cycle( e_lifecycle lifecycle ) override;
	bool is_fully_dead() override;
	void draw() override;
	void update() override;
	void post_spawn() override;
};

// ----------------------------------------------------------------------------

struct ec_sound : w_entity_component
{
	a_sound* snd = nullptr;

	ec_sound() = delete;
	ec_sound( w_entity* parent_entity );

	w_entity_component* init( const std::string_view snd_name );
	void draw() override;
};

// ----------------------------------------------------------------------------

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
	void draw() override;
};
