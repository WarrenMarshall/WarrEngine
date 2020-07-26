#pragma once

struct w_entity_component : i_lifecycle, i_transform
{
	e_component_type type = component_type::invalid;
	i_transform* parent_entity = nullptr;

	// used for offsetting whatever the component needs to
	float anim_offset = 0.0f;

	w_entity_component();
	virtual ~w_entity_component() = default;

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

	ec_sprite();

	w_entity_component* init( i_transform* parent_entity, a_subtexture* subtex );
	void draw() override;
};

// ----------------------------------------------------------------------------

struct ec_emitter : w_entity_component
{
	std::unique_ptr<w_particle_emitter> emitter = nullptr;

	ec_emitter();

	w_entity_component* init( i_transform* parent_entity, const std::string& params_name );
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

	ec_sound();

	w_entity_component* init( const std::string& snd_name );
	void draw() override;
};
